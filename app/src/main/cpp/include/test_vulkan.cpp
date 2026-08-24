#include <iostream>
#include <vector>
#include <MNN/Interpreter.hpp>

int main() {
    std::cout << "--- TEST MNN VULKAN INIZIATO ---" << std::endl;
    MNN::ScheduleConfig config;
    config.type = MNN_FORWARD_VULKAN;
    config.numThread = 4;
    std::cout << "Tentativo di inizializzazione..." << std::endl;
    
    auto runtime_pair = MNN::Interpreter::createRuntime({config});
    
    if (runtime_pair.second != nullptr) {
        std::cout << "SUCCESSO: La GPU ha accettato Vulkan!" << std::endl;
    } else {
        std::cout << "FALLITO: Impossibile creare il Runtime Vulkan." << std::endl;
    }
    
    return 0;
}

