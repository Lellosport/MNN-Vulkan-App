#include <iostream>
#include <chrono>
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>

int main(int argc, char* argv[]) {
    // Controlliamo che l'utente abbia passato un modello
    if (argc < 2) {
        std::cout << "Uso: ./test_vulkan <percorso_modello.mnn>" << std::endl;
        return 1;
    }

    std::cout << "--- BENCHMARK MNN VULKAN ---" << std::endl;
    const char* model_path = argv[1];

    // 1. Carica il modello
    auto interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path));
    if (!interpreter) {
        std::cout << "FALLITO: Impossibile leggere il file .mnn" << std::endl;
        return 1;
    }

    // 2. Configura Vulkan per le massime prestazioni
    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_VULKAN;
    config.numThread = 4;
    MNN::BackendConfig backendConfig;
    backendConfig.precision = MNN::BackendConfig::Precision_High;
    config.backendConfig = &backendConfig;

    // 3. Crea la sessione di calcolo
    auto session = interpreter->createSession(config);
    if (!session) {
        std::cout << "FALLITO: La GPU non riesce a creare la sessione." << std::endl;
        return 1;
    }

    std::cout << "Modello caricato! Riscaldamento GPU in corso..." << std::endl;
    // Eseguiamo un'inferenza a vuoto per riscaldare la memoria (warmup)
    interpreter->runSession(session);

    // 4. Inizia il cronometro!
    int loop_count = 50;
    std::cout << "Avvio benchmark (" << loop_count << " iterazioni)..." << std::endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < loop_count; ++i) {
        interpreter->runSession(session);
    }
    
    auto end = std::chrono::high_resolution_clock::now();

    // 5. Calcola i risultati
    std::chrono::duration<double, std::milli> ms_double = end - start;
    double avg_time = ms_double.count() / loop_count;

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Tempo TOTALE: " << ms_double.count() << " ms" << std::endl;
    std::cout << "Tempo MEDIO per singola inferenza: " << avg_time << " ms" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    return 0;
}
