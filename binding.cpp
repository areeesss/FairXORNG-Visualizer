#include <napi.h>
#include <vector>
#include <string>
#include "FairXORNG.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    // Convert C++ ProbabilityClass to JavaScript object
    auto createProbabilityClass = [](Napi::Env env, const ProbabilityClass& pc) {
        Napi::Object obj = Napi::Object::New(env);
        obj.Set("classLabel", pc.classLabel);
        obj.Set("initialProbability", pc.initialProbability);
        obj.Set("incrementalBiasRate", pc.incrementalBiasRate);
        obj.Set("consecutiveFailures", pc.consecutiveFailures);
        obj.Set("deterministicLimit", pc.deterministicLimit);
        
        Napi::Array categories = Napi::Array::New(env, pc.categoryOutcomes.size());
        for (size_t i = 0; i < pc.categoryOutcomes.size(); i++) {
            Napi::Object cat = Napi::Object::New(env);
            cat.Set("label", pc.categoryOutcomes[i].label);
            cat.Set("weight", pc.categoryOutcomes[i].weight);
            categories.Set(i, cat);
        }
        obj.Set("categoryOutcomes", categories);
        return obj;
    };

    // Convert JavaScript object to C++ ProbabilityClass
    auto parseProbabilityClass = [](Napi::Object obj) -> ProbabilityClass {
        ProbabilityClass pc;
        pc.classLabel = obj.Get("classLabel").As<Napi::String>().Utf8Value();
        pc.initialProbability = obj.Get("initialProbability").As<Napi::Number>().DoubleValue();
        pc.incrementalBiasRate = obj.Get("incrementalBiasRate").As<Napi::Number>().DoubleValue();
        pc.consecutiveFailures = obj.Get("consecutiveFailures").As<Napi::Number>().Int32Value();
        pc.deterministicLimit = obj.Get("deterministicLimit").As<Napi::Number>().Int32Value();
        
        Napi::Array categories = obj.Get("categoryOutcomes").As<Napi::Array>();
        for (uint32_t i = 0; i < categories.Length(); i++) {
            Napi::Object cat = categories.Get(i).As<Napi::Object>();
            OutcomeCategory oc;
            oc.label = cat.Get("label").As<Napi::String>().Utf8Value();
            oc.weight = cat.Get("weight").As<Napi::Number>().Int32Value();
            pc.categoryOutcomes.push_back(oc);
        }
        return pc;
    };

    // Expose the fairXORNG_Evaluate function
    exports.Set("evaluate", Napi::Function::New(env, [createProbabilityClass, parseProbabilityClass]
        (const Napi::CallbackInfo& info) -> Napi::Value {
            Napi::Env env = info.Env();
            
            if (info.Length() < 1 || !info[0].IsArray()) {
                Napi::TypeError::New(env, "Expected array of probability classes").ThrowAsJavaScriptException();
                return env.Null();
            }

            std::vector<ProbabilityClass> classes;
            Napi::Array jsClasses = info[0].As<Napi::Array>();
            
            for (uint32_t i = 0; i < jsClasses.Length(); i++) {
                classes.push_back(parseProbabilityClass(jsClasses.Get(i).As<Napi::Object>()));
            }

            std::string result = fairXORNG_Evaluate(classes);

            // Convert updated classes back to JavaScript
            Napi::Array updatedClasses = Napi::Array::New(env, classes.size());
            for (size_t i = 0; i < classes.size(); i++) {
                updatedClasses.Set(i, createProbabilityClass(env, classes[i]));
            }

            Napi::Object returnObj = Napi::Object::New(env);
            returnObj.Set("result", result);
            returnObj.Set("updatedClasses", updatedClasses);
            
            return returnObj;
        }));

    return exports;
}

NODE_API_MODULE(fairxorng, InitAll) 