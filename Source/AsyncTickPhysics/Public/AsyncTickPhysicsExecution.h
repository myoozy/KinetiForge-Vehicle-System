#pragma once

#include "CoreMinimal.h"

namespace Chaos { class FPhysicsSolverBase; }

namespace AsyncTickPhysics
{
	/** Borrowed only during NativeAsyncTick / Blueprint AsyncTick dispatch on this thread.
	 * Null outside the actual Chaos callback. Never cache this pointer. */
	ASYNCTICKPHYSICS_API Chaos::FPhysicsSolverBase* GetCurrentSolver();
}
