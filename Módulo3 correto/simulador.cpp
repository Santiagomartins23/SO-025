#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdexcept>
#include <iomanip>

class MemoryManager {
public:
    enum ReplacementAlgorithm { LRU, CLOCK };

    struct Page {
        int page_id;
        int process_id;
        bool referenced;
        bool modified;
        time_t last_used;
        bool present;
        int frame;
    };

    struct Frame {
        int frame_id;
        bool allocated;
        int page_id;
        int process_id;
    };

    struct Process {
        int process_id;
        int size;
        std::string status;
        std::unordered_map<int, Page> page_table;
        int swap_file_id;
    };

    struct MemoryOperation {
        int process_id;
        char operation_type; // 'R', 'W', 'P', 'I', 'C'
        unsigned long address;
        int size;
        std::string device;
    };

    MemoryManager(int page_size, int address_bits, int physical_mem, int secondary_mem, ReplacementAlgorithm algo)
        : page_size_(page_size),
          address_bits_(address_bits),
          physical_memory_size_(physical_mem),
          secondary_memory_size_(secondary_mem),
          replacement_algorithm_(algo) {

        validate_parameters();
        initialize_memory();
    }

    void simulate(const std::string& input_file) {
        std::ifstream file(input_file);
        if (!file) {
            throw std::runtime_error("Failed to open input file: " + input_file);
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            try {
                MemoryOperation op = parse_operation(line);
                handle_operation(op);
                display_system_status(op.process_id);
            } catch (const std::exception& e) {
                std::cerr << "Error processing line: " << line << "\n  Reason: " << e.what() << std::endl;
            }
        }

        print_summary();
    }

private:
    // Configurações do sistema
    const int page_size_;
    const int address_bits_;
    const int physical_memory_size_;
    const int secondary_memory_size_;
    const ReplacementAlgorithm replacement_algorithm_;

    // Estruturas de gerenciamento
    std::vector<Frame> physical_memory_;
    std::list<int> free_frames_;
    std::unordered_map<int, Process> processes_;
    std::queue<int> ready_queue_;

    // Algoritmos de substituição
    std::list<int> lru_list_;
    int clock_hand_ = 0;

    // Estatísticas
    int page_faults_ = 0;
    int swap_operations_ = 0;

    void validate_parameters() {
        if (page_size_ <= 0) throw std::invalid_argument("Page size must be positive");
        if (address_bits_ <= 0) throw std::invalid_argument("Address bits must be positive");
        if (physical_memory_size_ <= 0) throw std::invalid_argument("Physical memory size must be positive");
        if (secondary_memory_size_ <= 0) throw std::invalid_argument("Secondary memory size must be positive");
        if (physical_memory_size_ % page_size_ != 0) {
            throw std::invalid_argument("Physical memory size must be multiple of page size");
        }
    }

    void initialize_memory() {
        const int frame_count = physical_memory_size_ / page_size_;
        physical_memory_.resize(frame_count);

        for (int i = 0; i < frame_count; ++i) {
            physical_memory_[i] = {i, false, -1, -1};
            free_frames_.push_back(i);
        }
    }

    MemoryOperation parse_operation(const std::string& line) {
        std::istringstream iss(line);
        MemoryOperation op;
        char temp;

        // Formato: P1 C 500 ou P1 R (0)2
        iss >> temp >> op.process_id >> op.operation_type;

        if (op.operation_type == 'C') {
            iss >> op.size;
        } else {
            iss >> temp; // '('
            if (op.operation_type == 'I') {
                std::getline(iss, op.device, ')');
            } else {
                iss >> op.address;
                iss >> temp; // ')'
                iss >> temp; // '2' - indicador binário
            }
        }

        return op;
    }

    void handle_operation(const MemoryOperation& op) {
        switch (op.operation_type) {
            case 'C': create_process(op.process_id, op.size); break;
            case 'R':
            case 'W': handle_memory_access(op); break;
            case 'P': handle_cpu_instruction(op); break;
            case 'I': handle_io_operation(op); break;
            default:
                throw std::invalid_argument("Invalid operation type: " + std::string(1, op.operation_type));
        }
    }

    void create_process(int pid, int size) {
        if (processes_.count(pid)) {
            throw std::runtime_error("Process " + std::to_string(pid) + " already exists");
        }

        Process proc;
        proc.process_id = pid;
        proc.size = size;
        proc.status = "ready";
        proc.swap_file_id = -1;

        const int page_count = (size + page_size_ - 1) / page_size_;
        for (int i = 0; i < page_count; ++i) {
            proc.page_table[i] = {
                i,       // page_id
                pid,     // process_id
                false,   // referenced
                false,   // modified
                time(nullptr), // last_used
                false,   // present
                -1       // frame
            };
        }

        processes_[pid] = proc;
        ready_queue_.push(pid);

        std::cout << "Created process " << pid << " with " << page_count << " pages\n";
    }

    void handle_memory_access(const MemoryOperation& op) {
        auto& proc = get_process(op.process_id);
        const int page_num = op.address / page_size_;

        if (!proc.page_table.count(page_num)) {
            throw std::runtime_error("Invalid page number " + std::to_string(page_num) +
                                   " for process " + std::to_string(op.process_id));
        }

        auto& page = proc.page_table[page_num];
        const int frame = allocate_frame(proc, page);

        // Atualizar bits da página
        page.referenced = true;
        page.last_used = time(nullptr);
        if (op.operation_type == 'W') {
            page.modified = true;
        }

        // Atualizar estruturas do algoritmo de substituição
        update_replacement_structures(frame);

        std::cout << "Memory " << (op.operation_type == 'R' ? "read" : "write")
                  << " at address " << op.address << " (page " << page_num
                  << ", frame " << frame << ")\n";
    }

 int allocate_frame(Process& proc, Page& page) {
    if (page.present) return page.frame;

    if (!free_frames_.empty()) {
        int frame = free_frames_.front();
        free_frames_.pop_front();
        allocate_frame_to_page(proc, page, frame);
        load_page_from_secondary(proc.process_id, page.page_id, frame);
        return frame;
    }

    // Tentar liberar memória suspendendo processo
    try {
        swap_out_process();
    } catch (...) {
        std::cerr << "WARNING: Falha ao liberar memória com swapper\n";
    }

    // Depois do swap, tentar alocar novamente
    if (!free_frames_.empty()) {
        int frame = free_frames_.front();
        free_frames_.pop_front();
        allocate_frame_to_page(proc, page, frame);
        load_page_from_secondary(proc.process_id, page.page_id, frame);
        return frame;
    }

    // Se ainda assim não houver quadro, aplicar substituição
    int victim_frame = select_victim_frame();
    replace_page(victim_frame, proc, page);
    return victim_frame;
}

    int select_victim_frame() {
        switch (replacement_algorithm_) {
            case LRU: return select_lru_victim();
            case CLOCK: return select_clock_victim();
            default: throw std::logic_error("Invalid replacement algorithm");
        }
    }

    int select_lru_victim() {
        if (lru_list_.empty()) {
            throw std::runtime_error("LRU list is empty");
        }

        const int victim = lru_list_.back();
        lru_list_.pop_back();
        return victim;
    }

    int select_clock_victim() {
    int attempts = 0;
    while (attempts < physical_memory_.size()) {
        Frame& frame = physical_memory_[clock_hand_];
        if (frame.allocated) {
            Page& page = processes_[frame.process_id].page_table[frame.page_id];
            if (!page.referenced) {
                int victim = clock_hand_;
                clock_hand_ = (clock_hand_ + 1) % physical_memory_.size();
                return victim;
            }
            page.referenced = false;
        }
        clock_hand_ = (clock_hand_ + 1) % physical_memory_.size();
        ++attempts;
    }
    throw std::runtime_error("No frame found to evict using Clock");
}

void swap_out_process() {
    int target_pid = -1;
    size_t max_pages = 0;

    for (const auto& [pid, proc] : processes_) {
        size_t pages_in_mem = 0;
        for (const auto& [pg_id, pg] : proc.page_table) {
            if (pg.present) pages_in_mem++;
        }
        if (pages_in_mem > max_pages) {
            max_pages = pages_in_mem;
            target_pid = pid;
        }
    }

    if (target_pid == -1) throw std::runtime_error("No process to swap out");

    Process& proc = processes_[target_pid];
    for (auto& [pg_id, pg] : proc.page_table) {
        if (pg.present) {
            Frame& frame = physical_memory_[pg.frame];
            if (pg.modified) {
                save_page_to_secondary(proc.process_id, pg.page_id, frame.frame_id);
            }
            frame.allocated = false;
            frame.page_id = -1;
            frame.process_id = -1;
            pg.present = false;
            pg.frame = -1;
            free_frames_.push_back(frame.frame_id);
        }
    }
    proc.status = "suspended";
    std::cout << "Swapped out process " << target_pid << "\n";
}

    void replace_page(int victim_frame, Process& new_proc, Page& new_page) {
        Frame& frame = physical_memory_[victim_frame];

        // Salvar página vítima se foi modificada
        if (frame.allocated) {
            auto& victim_page = processes_[frame.process_id].page_table[frame.page_id];

            if (victim_page.modified) {
                save_page_to_secondary(frame.process_id, frame.page_id, victim_frame);
            }

            // Atualizar tabela de páginas do processo vítima
            victim_page.present = false;
            victim_page.frame = -1;
        }

        // Alocar para a nova página
        allocate_frame_to_page(new_proc, new_page, victim_frame);
        load_page_from_secondary(new_proc.process_id, new_page.page_id, victim_frame);

        page_faults_++;
        std::cout << "Page fault - Frame " << victim_frame << " replaced\n";
    }

    void allocate_frame_to_page(Process& proc, Page& page, int frame) {
        page.present = true;
        page.frame = frame;
        physical_memory_[frame] = {frame, true, page.page_id, proc.process_id};
    }

    void handle_cpu_instruction(const MemoryOperation& op) {
        std::cout << "Process " << op.process_id
                  << " executing CPU instruction: " << op.device << "\n";
    }

    void handle_io_operation(const MemoryOperation& op) {
        auto& proc = get_process(op.process_id);
        proc.status = "waiting_io";
        std::cout << "Process " << op.process_id
                  << " performing I/O on device: " << op.device << "\n";
    }

    Process& get_process(int pid) {
        auto it = processes_.find(pid);
        if (it == processes_.end()) {
            throw std::runtime_error("Process " + std::to_string(pid) + " not found");
        }
        return it->second;
    }

    void load_page_from_secondary(int pid, int page_id, int frame_id) {
        swap_operations_++;
        std::cout << "Loading page " << page_id << " of process " << pid
                  << " into frame " << frame_id << "\n";
    }

    void save_page_to_secondary(int pid, int page_id, int frame_id) {
        swap_operations_++;
        std::cout << "Saving page " << page_id << " of process " << pid
                  << " from frame " << frame_id << " to secondary storage\n";
    }

    void update_replacement_structures(int frame) {
        if (replacement_algorithm_ == LRU) {
            lru_list_.remove(frame);
            lru_list_.push_front(frame);
        }
    }

    void display_system_status(int current_pid) {
        display_memory_status();
        display_process_status(current_pid);
        std::cout << std::string(40, '-') << "\n";
    }

    void display_memory_status() {
        std::cout << "\nPhysical Memory Status:\n";
        std::cout << "Frame | Alloc | PID  | Page | Ref | Mod\n";
        std::cout << "------|-------|------|------|-----|----\n";

        for (const auto& frame : physical_memory_) {
            std::cout << std::setw(5) << frame.frame_id << " | ";
            std::cout << (frame.allocated ? "  Y  | " : "  N  | ");

            if (frame.allocated) {
                const auto& page = processes_.at(frame.process_id).page_table.at(frame.page_id);
                std::cout << std::setw(4) << frame.process_id << " | ";
                std::cout << std::setw(4) << frame.page_id << " | ";
                std::cout << (page.referenced ? " Y | " : " N | ");
                std::cout << (page.modified ? " Y" : " N");
            } else {
                std::cout << "     |      |     |    ";
            }
            std::cout << "\n";
        }
    }

    void display_process_status(int pid) {
        const auto& proc = get_process(pid);

        std::cout << "\nProcess " << pid << " Status:\n";
        std::cout << "Size: " << proc.size << " bytes | Status: " << proc.status << "\n";

        std::cout << "\nPage Table:\n";
        std::cout << "Page | Present | Frame | Ref | Mod | Last Used\n";
        std::cout << "-----|---------|-------|-----|-----|----------\n";

        for (const auto& [page_id, page] : proc.page_table) {
            std::cout << std::setw(4) << page_id << " | ";
            std::cout << (page.present ? "   Y   | " : "   N   | ");
            std::cout << (page.present ? std::setw(5) : std::setw(5)) << page.frame << " | ";
            std::cout << (page.referenced ? " Y | " : " N | ");
            std::cout << (page.modified ? " Y | " : " N | ");
            std::cout << std::put_time(std::localtime(&page.last_used), "%T");
            std::cout << "\n";
        }
    }
    void display_secondary_memory_status() {
    std::cout << "\n[Memória Secundária - Simulada]\n";
    std::cout << "Operacões de swap até agora: " << swap_operations_ << "\n";
}

    void print_summary() {
    std::cout << "\nSimulation Summary:\n";
    std::cout << "Total page faults: " << page_faults_ << "\n";
    std::cout << "Total swap operations: " << swap_operations_ << "\n";
    std::cout << "Active processes: " << processes_.size() << "\n";
    display_secondary_memory_status();
}
};

int main() {
    try {
        MemoryManager manager(
            4096,     // Page size (4KB)
            32,       // Address bits (32-bit)
            65536,    // Physical memory (64KB)
            1048576,  // Secondary memory (1MB)
            MemoryManager::CLOCK  // Replacement algorithm
        );

        manager.simulate("entrada.txt");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
