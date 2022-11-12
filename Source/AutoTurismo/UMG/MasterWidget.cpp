// Copyright Nat1Studios All Rights Reserved.


#include "MasterWidget.h"
/* Components */
#include "Components/TextBlock.h"
#include "Components/Button.h"
/* Utilities */
#include "Kismet/KismetStringLibrary.h"
#include "AutoTurismo/Subsystems/EventSubsystem.h"

#define TIME_TO_TEXT(t) FText::FromString(UKismetStringLibrary::TimeSecondsToString(t))

UMasterWidget* UMasterWidget::Instance = nullptr;

void UMasterWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GasButton->OnPressed.AddDynamic(this, &UMasterWidget::OnGasButtonDown);
	GasButton->OnReleased.AddDynamic(this, &UMasterWidget::OnGasButtonUp);
	ResetButton->OnClicked.AddDynamic(this, &UMasterWidget::OnResetButtonClicked);

	EventSubsystemAction(
		EventSubsystem->TimerResetDelegate.AddDynamic(this, &UMasterWidget::OnTimerReset);
		EventSubsystem->TimerUpdateDelegate.AddDynamic(this, &UMasterWidget::OnTimerUpdated);
		EventSubsystem->TimeSplitDelegate.AddDynamic(this, &UMasterWidget::OnTimeSplit);
		EventSubsystem->LapFinishDelegate.AddDynamic(this, &UMasterWidget::OnLapFinished);
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

void UMasterWidget::OnTimerReset()
{
	PreviousSplitTime = 0.0f;
	BestLapTime = BIG_NUMBER;
	TimerText->SetText(TIME_TO_TEXT(0.0f));
	SplitTimerText->SetText(TIME_TO_TEXT(0.0f));
	BestLapTimeText->SetText(FText::FromString("--:--.--"));
}

void UMasterWidget::OnTimerUpdated(float Time)
{
	TimerText->SetText(TIME_TO_TEXT(Time));
	SplitTimerText->SetText(TIME_TO_TEXT(Time - PreviousSplitTime));
}

void UMasterWidget::OnTimeSplit(float Time)
{
	PreviousSplitTime = Time;
}

void UMasterWidget::OnLapFinished(float Time)
{
	if (Time < BestLapTime)
	{
		BestLapTime = Time;
		BestLapTimeText->SetText(TIME_TO_TEXT(BestLapTime));
	}
}
#pragma endregion