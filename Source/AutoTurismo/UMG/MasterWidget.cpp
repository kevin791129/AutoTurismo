// Copyright Nat1Studios All Rights Reserved.


#include "MasterWidget.h"
/* Components */
#include "Components/TextBlock.h"
#include "Components/Button.h"
/* Utilities */
#include "Kismet/KismetStringLibrary.h"
#include "AutoTurismo/Subsystems/EventSubsystem.h"

UMasterWidget* UMasterWidget::Instance = nullptr;

void UMasterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GasButton->OnPressed.AddDynamic(this, &UMasterWidget::OnGasButtonDown);
	GasButton->OnReleased.AddDynamic(this, &UMasterWidget::OnGasButtonUp);
	ResetButton->OnClicked.AddDynamic(this, &UMasterWidget::OnResetButtonClicked);

	EventSubsystemAction(
		EventSubsystem->TimerUpdateDelegate.AddDynamic(this, &UMasterWidget::OnTimerUpdated);
	)
}

void UMasterWidget::NativeConstruct()
{
	Instance = this;
}

void UMasterWidget::NativeDestruct()
{
	if (Instance == this)
		Instance = nullptr;
}

#pragma region Delegate Binding
void UMasterWidget::OnGasButtonDown()
{
	EventSubsystemAction(
		if (EventSubsystem->ThrottleInputPressedDelegate.IsBound())
			EventSubsystem->ThrottleInputPressedDelegate.Broadcast();
	)
}

void UMasterWidget::OnGasButtonUp()
{
	EventSubsystemAction(
		if (EventSubsystem->ThrottleInputReleasedDelegate.IsBound())
			EventSubsystem->ThrottleInputReleasedDelegate.Broadcast();
	)
}

void UMasterWidget::OnResetButtonClicked()
{
	EventSubsystemAction(
		if (EventSubsystem->ResetVehicleDelegate.IsBound())
			EventSubsystem->ResetVehicleDelegate.Broadcast();
	)
}

void UMasterWidget::OnTimerUpdated(float Timer)
{
	TimerText->SetText(FText::FromString(UKismetStringLibrary::TimeSecondsToString(Timer)));
}
#pragma endregion