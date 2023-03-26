// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

#include "CoreLocalPlayerController.generated.h"

/**
 * 
 */

UCLASS()
class RAIDPARTY_API ACoreLocalPlayerController : public APlayerController
{
	GENERATED_BODY()
	ACoreLocalPlayerController();

	int32 CurrentPathIndex{ 0 };
	int32 MaxPathIndex { 0 };

public:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	class ABoardPlayerState* State{ nullptr };

	UPROPERTY(BlueprintReadWrite)
	class ABoardPawn* myPawn {nullptr};

	int32 PlayerIndex{ -1 };

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 MyRoll {-1};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMapping{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* ConfirmAction{ nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	class UInputAction* PathSelectAction{ nullptr };

	UFUNCTION(BlueprintCallable)
	void PawnArrived(class ABoardSpace* space);


protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;
	void EndTurnOnSpace(class ABoardSpace* space);
	UPROPERTY(BlueprintReadWrite)
	bool bIsMyTurn {false};

	UPROPERTY(BlueprintReadWrite)
	bool bSelectingPaths{ false };

	UPROPERTY(BlueprintReadWrite)
	bool bRolled{ false };


	UPROPERTY(BlueprintReadWrite)
	bool bWaitingForConfirmation{ false };

	TQueue<TFunction<void()>> ConfirmQueue;

	UFUNCTION(BlueprintImplementableEvent)
	void FinishedMoving();

	UFUNCTION(BlueprintCallable)
	void RollDice();

	// Input-bound Functions
	UFUNCTION()
	void Confirm(const FInputActionValue& Value);

	UFUNCTION()
	void SelectPath(const FInputActionValue& Value);
};


