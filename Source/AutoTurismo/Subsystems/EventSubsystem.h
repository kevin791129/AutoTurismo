// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EventSubsystem.generated.h"

#define EventSubsystemAction(x) { UEventSubsystem* EventSubsystem = GetWorld()->GetSubsystem<UEventSubsystem>(); if (IsValid(EventSubsystem)) { x } } 

/**
 * Event manager.
 */
UCLASS()
class AUTOTURISMO_API UEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleEvent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimerUpdateEvent, float, Timer);

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (DisplayName = "On Simulate Throttle Pressed"))
		FSimpleEvent ThrottleInputPressedDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (DisplayName = "On Simulate Throttle Released"))
		FSimpleEvent ThrottleInputReleasedDelegate;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (DisplayName = "On Vehicle Reset Triggered"))
		FSimpleEvent ResetVehicleDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, meta = (DisplayName = "On Timer Updated"))
		FTimerUpdateEvent TimerUpdateDelegate;
};
