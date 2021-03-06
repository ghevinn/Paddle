/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include "paddle/fluid/framework/executor.h"
#include "paddle/fluid/framework/op_info.h"
#include "paddle/fluid/framework/program_desc.h"
#include "paddle/fluid/framework/scope.h"
#include "paddle/fluid/framework/tensor.h"
#include "paddle/fluid/platform/device_context.h"

namespace paddle {
namespace framework {

class ParallelExecutorPrivate;

class ParallelExecutor {
  DISABLE_COPY_AND_ASSIGN(ParallelExecutor);

 public:
  explicit ParallelExecutor(size_t num_threads, bool use_event,
                            const std::vector<platform::Place>& places,
                            const std::unordered_set<std::string>& params,
                            const std::unordered_set<std::string>& bcast_vars,
                            const ProgramDesc& main_program,
                            const std::string& loss_var_name, Scope* scope,
                            const std::vector<Scope*>& local_scopes,
                            bool allow_op_delay);

  std::vector<Scope*>& GetLocalScopes();

  /**
   * Feed tensors to local scopes. The size of tensors should be equal to the
   * size of local scopes.
   */
  void FeedTensorsIntoLocalScopes(
      const std::vector<std::unordered_map<std::string, LoDTensor>>& tensors);

  void FeedAndSplitTensorIntoLocalScopes(
      const std::unordered_map<std::string, LoDTensor>& tensors);

  void Run(const std::vector<std::string>& fetch_tensors,
           const std::string& fetched_var_name);

  void BCastParamsToGPUs(const std::unordered_set<std::string>& vars) const;

 private:
  ParallelExecutorPrivate* member_;
};

}  // namespace framework
}  // namespace paddle
