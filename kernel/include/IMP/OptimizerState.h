/**
 *  \file OptimizerState.h   \brief Shared optimizer state.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef __IMP_OPTIMIZER_STATE_H
#define __IMP_OPTIMIZER_STATE_H

#include <iostream>

#include "IMP_config.h"
#include "VersionInfo.h"
#include "internal/Object.h"
#include "internal/ObjectPointer.h"
#include "Optimizer.h"

namespace IMP
{

class Optimizer;

//! Shared optimizer state.
class IMPDLLEXPORT OptimizerState : public internal::Object
{
  friend class Optimizer;
  void set_optimizer(Optimizer* optimizer);

public:
  OptimizerState(std::string name=std::string());
  virtual ~OptimizerState();

  // Update the state given the current state of the optimizer
  virtual void update() = 0;

  virtual void show(std::ostream& out = std::cout) const;

  //! \return version and authorship information.
  virtual VersionInfo get_version_info() const {
    return VersionInfo("unknown", "unknown");
  }

  //! Get the name of the state
  const std::string& get_name() const {
    return name_;
  }
  //! Set the name of the state
  void set_name(const std::string &name) {
    name_=name;
  }

  //! return the stored optimizer
  Optimizer *get_optimizer() const {
    IMP_assert(optimizer_,
               "Must call set_optimizer before get_optimizer on state");
    return optimizer_.get();
  }
protected:
  //! Stored optimizer
  internal::ObjectPointer<Optimizer, false> optimizer_;

  std::string name_;
};


inline std::ostream &operator<<(std::ostream &out, const OptimizerState &s)
{
  s.show(out);
  return out;
}

} // namespace IMP

#endif  /* __IMP_OPTIMIZER_STATE_H */
