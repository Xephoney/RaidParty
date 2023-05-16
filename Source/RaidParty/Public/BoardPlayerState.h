// Laget av Hans Ola Hoftun


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BoardPlayerState.generated.h"

//typedef TArray<int> RollMods;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiceRolled, int32, PlayerRollOriginal, const TArray<int32>&, Modifications);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoinsChanged, int32, Original, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeepsChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRankChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FColorChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyRollChanged)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerEndTurn, int32, PlayerIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerBeginTurn, int32, PlayerIndex);


/**
 * 
 */
UCLASS()
class RAIDPARTY_API ABoardPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateState();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	int32 RollDice();

	UFUNCTION(BlueprintCallable)
		void SetCoins(int32 newCoinsBalance);

	float dt;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 Coins {10};
	int32 oldCoins = 0;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Keeps{ 0 };
	uint8 oldKeeps = 5;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FLinearColor Color{1,1,1,1};
	FLinearColor oldColor = FLinearColor(0, 0, 0, 0);

	UPROPERTY(BlueprintReadWrite, SaveGame)
	uint8 Ranking{ 1 };
	uint8 oldRanking = 5;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 BoardIndex ;

	//Controller Variables
	UPROPERTY(BlueprintReadWrite)
	int32 MyRoll;
	int32 OldRoll;
	UPROPERTY(BlueprintReadWrite)
	int32 PlayerIndex{ -1 };

	UPROPERTY(BlueprintReadWrite)
	class ABoardPawn* myPawn;

	UPROPERTY(BlueprintReadWrite)
		class ABoardTurnCharacter* TurnCharacter{ nullptr };

	UPROPERTY(BlueprintReadWrite)
		bool bRolled{ false };

	UPROPERTY(BlueprintReadWrite)
		bool bRolling{ false };

	UPROPERTY(BlueprintReadWrite)
		bool bIsMyTurn{ false };

	UPROPERTY(BlueprintReadWrite)
		bool bSelectingPaths{ false };
	UPROPERTY(BlueprintReadWrite)
		bool bRollMode{ false };

	UPROPERTY(BlueprintReadWrite)
		bool bCameraMode{ false };

	UPROPERTY(BlueprintReadWrite)
		bool bPostRollPreMove{ false };

	UPROPERTY(BlueprintAssignable)
		FOnCoinsChanged OnCoinsChanged;

	UPROPERTY(BlueprintAssignable)
	FOnKeepsChangedDelegate OnKeepsChanged;
	
	UPROPERTY(BlueprintAssignable)
	FColorChangedDelegate OnColorChanged;

	UPROPERTY(BlueprintAssignable)
	FRankChangedDelegate OnRankingChanged;

	UPROPERTY(BlueprintAssignable)
	FMyRollChanged OnMyRollChanged;
	
	// Call functions for Setting up and preparing players for their turn
	UFUNCTION(BlueprintCallable)
	void BeginTurn(class ABoardTurnCharacter* inCharacter);

	UFUNCTION(BlueprintCallable)
	void EndTurn();

	UPROPERTY(BlueprintReadWrite)
	bool bAI = true;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerBeginTurn BeginTurnDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerEndTurn EndTurnDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDiceRolled OnDiceRolledDelegate;

	void AddEffect(int EffectIndex, int shrineIndex);

	void RemoveEffect(int EffectIndex, int shrineIndex);


	TArray<std::tuple<int32, int32, int32>> ShrineEffectLinkArray;

	// Effect Arrays

	// On Dice Roll
	TArray<TFunction<int(ABoardPlayerState&)>> DiceRollEffects;

	// When the turn starts
	TArray<TFunction<int(ABoardPlayerState&)>> TurnStartEffects;

	// When the turn ends
	TArray<TFunction<int(ABoardPlayerState&)>> TurnEndEffects;

	// Coins Change effects
	TArray<TFunction<int(ABoardPlayerState&)>> CoinsChangedEffects;

};
