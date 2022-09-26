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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFollowInputPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFollowInputReleased);

public:
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Simulate Follow Pressed"))
		FFollowInputPressed FollowInputPressedDelegate;
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On Simulate Follow Released"))
		FFollowInputReleased FollowInputReleasedDelegate;
	
};
