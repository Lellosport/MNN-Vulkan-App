#include <iostream>
#include <chrono>
#include <vector>
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: ./test_vulkan <modello_video.mnn>" << std::endl;
        return 1;
    }

    std::cout << "--- INIZIALIZZAZIONE MODELLO VIDEO MNN ---" << std::endl;
    const char* model_path = argv[1];

    // 1. Caricamento dell'interprete
    auto interpreter = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path));
    if (!interpreter) {
        std::cout << "ERRORE: Impossibile caricare il file del modello .mnn" << std::endl;
        return 1;
    }

    // 2. Configurazione di Vulkan per le massime prestazioni
    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_VULKAN;
    config.numThread = 4;
    
    MNN::BackendConfig backendConfig;
    backendConfig.precision = MNN::BackendConfig::Precision_High;
    config.backendConfig = &backendConfig;

    // 3. Creazione della sessione di calcolo sulla GPU
    auto session = interpreter->createSession(config);
    if (!session) {
        std::cout << "ERRORE: Impossibile creare la sessione Vulkan per il modello." << std::endl;
        return 1;
    }

    std::cout << "SUCCESSO: Modello caricato e sessione Vulkan avviata!" << std::endl;

    // 4. Recupero dei tensori di input
    MNN::Tensor* inputTensor = interpreter->getSessionInput(session, nullptr);
    std::cout << "Dimensioni input tensor pronte per l'elaborazione." << std::endl;

    // 5. Test di inferenza rapida con namespace corretto
    auto start = std::chrono::high_resolution_clock::now();
    
    MNN::ErrorCode status = interpreter->runSession(session);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    if (status == 0) { // NO_ERROR
        std::cout << "INFERENZA RIUSCITA!" << std::endl;
        std::cout << "Tempo impiegato per il passaggio: " << elapsed.count() << " ms" << std::endl;
    } else {
        std::cout << "ERRORE durante l'esecuzione dell'inferenza. Codice: " << status << std::endl;
    }

    return 0;
}
