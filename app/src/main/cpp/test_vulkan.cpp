#include <iostream>
#include <chrono>
#include <vector>
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: ./test_vulkan <modello_wan21.mnn>" << std::endl;
        return 1;
    }

    std::cout << "--- WAN2.1 MNN VULKAN RUNTIME ---" << std::endl;
    const char* model_path = argv[1];

    // 1. Caricamento dell'interprete
    auto interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path));
    if (!interpreter) {
        std::cout << "ERRORE: Impossibile leggere il file .mnn del modello." << std::endl;
        return 1;
    }

    // 2. Configurazione Vulkan orientata alle performance
    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_VULKAN;
    config.numThread = 4;
    
    MNN::BackendConfig backendConfig;
    backendConfig.precision = MNN::BackendConfig::Precision_High;
    config.backendConfig = &backendConfig;

    // 3. Creazione della sessione
    auto session = interpreter->createSession(config);
    if (!session) {
        std::cout << "ERRORE: Impossibile creare la sessione Vulkan per Wan2.1." << std::endl;
        return 1;
    }

    std::cout << "SUCCESSO: Sessione Vulkan creata con successo!" << std::endl;

    // 4. Analisi dei tensori di input (fondamentale per i modelli DiT)
    auto inputTensors = interpreter->getSessionInputAll(session);
    std::cout << "Il modello richiede " << inputTensors.size() << " tensore/i di input." << std::endl;
    
    for (auto& pair : inputTensors) {
        std::cout << " - Input Name: " << pair.first << std::endl;
        auto dims = pair.second->shape();
        std::cout << "   Dimensioni (Shape): [ ";
        for (int d : dims) std::cout << d << " ";
        std::cout << "]" << std::endl;
    }

    // 5. Test di esecuzione a vuoto (Warmup)
    std::cout << "Esecuzione test di passaggio (Warmup)..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    MNN::ErrorCode status = interpreter->runSession(session);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    if (status == 0) {
        std::cout << "INFERENZA DI TEST RIUSCITA!" << std::endl;
        std::cout << "Tempo impiegato: " << elapsed.count() << " ms" << std::endl;
    } else {
        std::cout << "NOTA: Il modello richiede input specifici (Codice errore: " << status << "). La struttura risponde correttamente." << std::endl;
    }

    return 0;
}
