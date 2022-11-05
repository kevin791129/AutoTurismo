// Copyright Nat1Studios All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MasterWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * Master widget.
 */
UCLASS()
class AUTOTURISMO_API UMasterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * Get master widget singleton.
	 *
	 * @return Master widget instance.
	 */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Master Widget"))
		static UMasterWidget* GetInstance() { return Instance; }

protected:
	// UUserWidget
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~UUserWidget

#pragma region Delegate Binding
	/**
	 * Delegate binding on gas button pressed. 
	 */
	UFUNCTION()
		void OnGasButtonDown();
	/**
	 * Delegate binding on gas button released.
	 */
	UFUNCTION()
		void OnGasButtonUp();
	/**
	 * Delegate binding on gas button released.
	 */
	UFUNCTION()
		void OnResetButtonClicked();
	/**
	 * Delegate binding on timer is updated.
	 * 
	 * @param Timer : Updated timer time.
	 */
	UFUNCTION()
		void OnTimerUpdated(float Timer);
#pragma endregion

protected:
	/* Timer text block*/
	UPROPERTY(meta = (BindWidget))
		UTextBlock* TimerText;
	/* Gas button*/
	UPROPERTY(meta = (BindWidget))
		UButton* GasButton;
	/* Reset button*/
	UPROPERTY(meta = (BindWidget))
		UButton* ResetButton;

private:
	/* Singleton instance*/
	static UMasterWidget* Instance;
};
