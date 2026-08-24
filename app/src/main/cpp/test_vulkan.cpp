#include <iostream>
#include <chrono>
#include <MNN/Interpreter.hpp>

int main() {
    std::cout << "--- BENCHMARK VULKAN PURO ---" << std::endl;

    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_VULKAN;
    config.numThread = 4;

    int loop_count = 100;
    std::cout << "Avvio test di creazione contesto Vulkan (" << loop_count << " iterazioni)..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < loop_count; ++i) {
        auto runtime_pair = MNN::Interpreter::createRuntime({config});
        if (runtime_pair.second == nullptr) {
            std::cout << "Errore Vulkan alla iterazione " << i << std::endl;
            return 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> ms_double = end - start;
    double avg_time = ms_double.count() / loop_count;

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Tempo TOTALE: " << ms_double.count() << " ms" << std::endl;
    std::cout << "Tempo MEDIO per inizializzazione: " << avg_time << " ms" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    return 0;
}
