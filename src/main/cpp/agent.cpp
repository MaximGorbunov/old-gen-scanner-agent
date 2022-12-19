#include <jni.h>
#include <jvmti.h>
#include <memory>
#include <vector>

#include "vm/jvm.h"
#include "symbolsParser.h"
#include "vm/models/g1Heap.h"

using namespace std;

void JNICALL GarbageCollectionFinish(jvmtiEnv *jvmti_env) {
    auto symbolsParser = make_shared<SymbolsParser>();
    shared_ptr<JVM> jvm(new JVM(symbolsParser));
    shared_ptr<JvmTypesContainer> jvmTypesContainer(new JvmTypesContainer(jvm));
    char *g1CollectedHeap = (char *) jvm->getG1CollectedHeap();
    G1Heap g1(g1CollectedHeap, jvmTypesContainer);
    g1.iterate(jvm, jvmTypesContainer);
}

extern jint JNICALL Agent_OnLoad(JavaVM * vm, char * options, void * reserved) {
    jvmtiEnv *jvmti_env;
    jvmtiCapabilities capabilities{};
    capabilities.can_generate_garbage_collection_events = true;
    vm->GetEnv((void **) &jvmti_env, JVMTI_VERSION_11);
    jvmtiEventCallbacks callbacks = {nullptr};
    callbacks.GarbageCollectionFinish = GarbageCollectionFinish;
    jvmti_env->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti_env->AddCapabilities(&capabilities);
    jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_GARBAGE_COLLECTION_FINISH, nullptr);
    return 0;
}
