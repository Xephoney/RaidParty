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

	int32 CurrentPathIndex;
	int32 MaxPathIndex;
	
public:
	ABoardAIController();
	UPROPERTY(BlueprintReadWrite, SaveGame)
	class ABoardPlayerState* State{ nullptr };

	UFUNCTION(BlueprintImplementableEvent)
	void BeginRollDice();

	UFUNCTION(BlueprintImplementableEvent)
	void SelectPath();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRoll();

	UFUNCTION(BlueprintImplementableEvent)
	void BeginTurn(int32 playerIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void EndTurn(int32 playerIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void FinishedMoving(SPACETYPE Type);

	UFUNCTION(BlueprintImplementableEvent)
	void ArrivedAtSpace(class ABoardSpace* space);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerHalted(SPACETYPE Type);
	
	UFUNCTION(BlueprintCallable)
	void PawnArrived(class ABoardSpace* space);

	UFUNCTION(BlueprintCallable)
	void ContinueMovement();

	UFUNCTION(BlueprintCallable)
	void RollComplete();

	void ActivatePathSelect(const ABoardSpace& space);

	UFUNCTION(BlueprintCallable)
		void SelectNewPath();

	UFUNCTION(BlueprintCallable)
	void PathSelected();



	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

	float elapsed = 0.f;
	float timeSpentRolling = 0.f;
};
