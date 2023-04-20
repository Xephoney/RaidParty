// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BoardAIController.generated.h"

/**
 * 
 */
enum class SPACETYPE : uint8;
UCLASS()
class RAIDPARTY_API ABoardAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABoardAIController();
	UPROPERTY(BlueprintReadWrite, SaveGame)
	class ABoardPlayerState* State{ nullptr };

	UPROPERTY(BlueprintReadWrite)
	class ABoardPawn* myPawn {nullptr};

	UPROPERTY(BlueprintReadWrite)
	class ABoardPawn* TurnCharacter{ nullptr };

	UFUNCTION(BlueprintImplementableEvent)
	void BeginRollDice();

	UFUNCTION(BlueprintImplementableEvent)
	void SelectPath();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRoll();

	UFUNCTION(BlueprintImplementableEvent)
	void EndTurn();

	UFUNCTION(BlueprintImplementableEvent)
		void FinishedMoving(SPACETYPE Type);

	UFUNCTION(BlueprintImplementableEvent)
		void ArrivedAtSpace(class ABoardSpace* space);

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerHalted(SPACETYPE Type);

	

	UFUNCTION(BlueprintCallable)
	void PawnArrived(class ABoardSpace* space);

	UFUNCTION(BlueprintCallable)
	void ContinueMovement();

	void ActivatePathSelect(const ABoardSpace& space);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	


	UPROPERTY(BlueprintReadWrite)
	bool bIsMyTurn =false;
	bool bSelectingPaths = true;
	bool bRolled = true;

	UPROPERTY(BlueprintReadWrite)
	int MyRoll = 0;

};
