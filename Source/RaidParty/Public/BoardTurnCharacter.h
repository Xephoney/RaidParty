// Laget av Hans Ola Hoftun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoardTurnCharacter.generated.h"

UCLASS()
class RAIDPARTY_API ABoardTurnCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoardTurnCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCamera(FVector2D direction);

	UPROPERTY(BlueprintReadWrite)
	AActor* FollowTarget{ nullptr };

	UPROPERTY(BlueprintReadWrite)
	bool bFreeCameraMode = false;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RollDiceModeActivated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DefaultMode();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LookAroundMode();

};
