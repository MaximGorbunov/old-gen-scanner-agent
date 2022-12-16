#include <jni.h>
#include <jvmti.h>
#include <memory>
#include <thread>

#include "vm/jvm.h"
#include "vm/models/g1Heap.h"


using namespace std;

void task() {
    std::this_thread::sleep_for(500ms);
    shared_ptr<JVM> jvm(new JVM());
    shared_ptr<JvmTypesContainer> jvmTypesContainer(new JvmTypesContainer(jvm));
    char *g1CollectedHeap = (char*) jvm->getG1CollectedHeap();
    G1Heap g1(g1CollectedHeap, jvmTypesContainer);
    g1.iterate(jvm, jvmTypesContainer);
}

void JNICALL VMStart(jvmtiEnv *jvmti_env, JNIEnv *jni_env) {
    new std::thread(task);
}

extern jint JNICALL Agent_OnLoad(JavaVM * vm, char * options, void * reserved) {
    jvmtiEnv *jvmti_env;
    vm->GetEnv((void **) &jvmti_env, JVMTI_VERSION_11);
    jvmtiEventCallbacks callbacks = {nullptr};
    callbacks.VMStart = VMStart;
    jvmti_env->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_VM_START, nullptr);
    return 0;
}
