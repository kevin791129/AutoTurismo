// Copyright Nat1Studios All Rights Reserved.


#include "MasterWidget.h"
#include "Components/Button.h"
#include "AutoTurismo/Subsystems/EventSubsystem.h"

UMasterWidget* UMasterWidget::Instance = nullptr;

void UMasterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GasButton->OnPressed.AddDynamic(this, &UMasterWidget::OnGasButtonDown);
	GasButton->OnReleased.AddDynamic(this, &UMasterWidget::OnGasButtonUp);
}

void UMasterWidget::NativeConstruct()
{
	Instance = this;
}

void UMasterWidget::NativeDestruct()
{
	Instance = nullptr;
}

void UMasterWidget::OnGasButtonDown()
{
	EventSubsystemAction(
		if (EventSubsystem->FollowInputPressedDelegate.IsBound())
			EventSubsystem->FollowInputPressedDelegate.Broadcast();
	)
}

void UMasterWidget::OnGasButtonUp()
{
	EventSubsystemAction(
		if (EventSubsystem->FollowInputReleasedDelegate.IsBound())
			EventSubsystem->FollowInputReleasedDelegate.Broadcast();
	)
}