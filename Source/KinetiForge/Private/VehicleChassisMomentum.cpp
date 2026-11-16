// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#include "VehicleChassisMomentum.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

#include <type_traits>
#include <utility>

namespace VehicleChassisMomentumPrivate
{
	// Newer Chaos versions have an opt-in native gyro integrator. Detect the API instead
	// of guessing an engine version; do not apply a second correction when it is enabled.
	template <typename T, typename = void>
	struct TNativeGyro
	{
		static bool IsEnabled(const T&) { return false; }
	};
	template <typename T>
	struct TNativeGyro<T, std::void_t<decltype(std::declval<const T&>().GyroscopicTorqueEnabled())>>
	{
		static bool IsEnabled(const T& Body) { return Body.GyroscopicTorqueEnabled(); }
	};

	FVector Momentum(const FQuat& Rotation, const FVector& Inertia, const FVector& Velocity)
	{
		return Rotation.RotateVector(Inertia * Rotation.UnrotateVector(Velocity));
	}
}

void FVehicleChassisMomentum::Reset()
{
	PreviousBody.Reset();
	bHasHistory = false;
}

void FVehicleChassisMomentum::Apply(Chaos::FRigidBodyHandle_Internal* Body)
{
	using namespace VehicleChassisMomentumPrivate;
	if (!Body || !Body->CanTreatAsRigid() || TNativeGyro<Chaos::FRigidBodyHandle_Internal>::IsEnabled(*Body))
	{
		Reset();
		return;
	}
	const auto Identity = Body->GetProxy()->GetSyncTimestamp();
	const FVector Inertia(Body->I()); // kg*cm^2, principal mass frame (not actor frame).
	const FQuat MassRotation = (Body->R() * Body->RotationOfMass()).GetNormalized();
	if (bHasHistory && PreviousBody == Identity && PreviousInertia.Equals(Inertia, 0.))
	{
		// Chaos 5.3 carries world W through an orientation update. Preserve the corresponding
		// world angular momentum instead: add (I_previous_world - I_current_world) * W.
		// Use current W so damping/external velocity changes are retained, rather than restoring
		// a cached total momentum. Pending wheel/contact/aero impulses are left intact.
		const FVector W(Body->W());
		const FVector Correction = Momentum(PreviousMassRotation, Inertia, W)
			- Momentum(MassRotation, Inertia, W);
		if (!Correction.IsNearlyZero())
		{
			Body->SetAngularImpulse(Body->AngularImpulse() + Correction, false);
		}
	}
	PreviousBody = Identity;
	PreviousMassRotation = MassRotation;
	PreviousInertia = Inertia;
	bHasHistory = true;
}
