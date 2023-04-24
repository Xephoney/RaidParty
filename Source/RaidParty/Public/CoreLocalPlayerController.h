// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

#include "CoreLocalPlayerController.generated.h"

/**
 * 
 */

enum class SPACETYPE : uint8;
UCLASS()
class RAIDPARTY_API ACoreLocalPlayerController : public APlayerController
{
	GENERATED_BODY()
	ACoreLocalPlayerController();

	int32 CurrentPathIndex{ 0 };
	int32 MaxPathIndex { 0 };
	TArray<FVector2D> PathDirections;
	float elapsed = 0.f;
	bool bRightJoystickReset = false;
public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	class ABoardPlayerState* State{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMapping{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* ConfirmAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* CancelAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* ConfirmReleasedAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* RollDiceAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* CameraToggleAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* PathSelectHorizontalAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* PathSelectVerticalAction{ nullptr };


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* DirectionalAction{ nullptr };

	void PawnArrived(class ABoardSpace* space);

	UFUNCTION(BlueprintCallable)
	void ContinueMovement();

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	

	UPROPERTY(BlueprintReadWrite)
	bool bWaitingForConfirmation{ false };

	TArray<TFunction<void()>> ConfirmStack;
	TArray<TFunction<void()>> DeclineStack;
	
	UFUNCTION(BlueprintImplementableEvent)
	void FinishedMoving(SPACETYPE Type);

	UFUNCTION(BlueprintImplementableEvent)
	void ArrivedAtSpace(ABoardSpace* space);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHalted(SPACETYPE Type);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRoll();

	UFUNCTION(BlueprintImplementableEvent)
	void EndTurn();

	void RollDiceBegin();
	void RollDice();

	void CancelActivated();

	void ActivatePathSelect(const ABoardSpace& space);
	// Input-bound Functions
	UFUNCTION()
	void Confirm(const FInputActionValue& Value);

	UFUNCTION()
	void ConfirmReleased(const FInputActionValue& Value);

	UFUNCTION()
	void CameraModeToggle(const FInputActionValue& Value);

	UFUNCTION()
	void SelectPathHorizontal(const FInputActionValue& Value);
	void SelectPathVertical(const FInputActionValue& Value);

	UFUNCTION()
	void JoystickInput(const FInputActionValue& Value);

	void SelectPathFromDirection(FVector2D Direction);
	void SelectShrineFromDirection(FVector2D Direction);
	UFUNCTION(BlueprintCallable)
	void StartSelectingShrineOptions();

};


