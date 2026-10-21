// Copyright (c) 2026 Zhengyi Miao (github.com/myoozy)

#pragma once

#include "CoreMinimal.h"

struct FBodyInstance;
struct FProxyTimestampBase;
class FVehiclePhysicsExecutionContext;
namespace Chaos { class FRigidBodyHandle_Internal; }

/** Immutable cross-thread identity copied on GT; deliberately contains no UObject or proxy pointer. */
class KINETIFORGE_API FVehiclePhysicsBodyIdentity
{
public:
	FVehiclePhysicsBodyIdentity() = default;
	/** GT-only. The BodyInstance must remain valid for this call; only its shared timestamp is retained. */
	static FVehiclePhysicsBodyIdentity Capture(const FBodyInstance* BodyInstance);
	/** Indicates a token was captured, not that the associated body still exists. */
	bool IsValid() const { return Timestamp.IsValid(); }
	bool operator==(const FVehiclePhysicsBodyIdentity& Other) const { return Timestamp == Other.Timestamp; }
	bool operator!=(const FVehiclePhysicsBodyIdentity& Other) const { return !(*this == Other); }

private:
	TSharedPtr<FProxyTimestampBase, ESPMode::ThreadSafe> Timestamp;
	friend class FVehiclePhysicsExecutionContext;
};

/** Borrowed body lookups for one vehicle macrostep within a physics callback.
 * Create on the stack and discard at step end. Other vehicles may use separate instances concurrently. */
class KINETIFORGE_API FVehiclePhysicsMacrostepBodyResolver
{
public:
	explicit FVehiclePhysicsMacrostepBodyResolver(const FVehiclePhysicsExecutionContext& InContext)
		: Context(InContext) {}
	FVehiclePhysicsMacrostepBodyResolver(const FVehiclePhysicsMacrostepBodyResolver&) = delete;
	FVehiclePhysicsMacrostepBodyResolver& operator=(const FVehiclePhysicsMacrostepBodyResolver&) = delete;
	FVehiclePhysicsMacrostepBodyResolver(FVehiclePhysicsMacrostepBodyResolver&&) = delete;
	FVehiclePhysicsMacrostepBodyResolver& operator=(FVehiclePhysicsMacrostepBodyResolver&&) = delete;

	Chaos::FRigidBodyHandle_Internal* ResolveBody(const FVehiclePhysicsBodyIdentity& Identity);
	/** Number of calls delegated to the underlying solver scan; useful for focused regression checks. */
	int32 GetUnderlyingResolveCount() const { return UnderlyingResolveCount; }

private:
	struct FEntry
	{
		FVehiclePhysicsBodyIdentity Identity;
		Chaos::FRigidBodyHandle_Internal* Handle = nullptr;
	};
	const FVehiclePhysicsExecutionContext& Context;
	TArray<FEntry, TInlineAllocator<4>> Entries;
	int32 UnderlyingResolveCount = 0;
};
