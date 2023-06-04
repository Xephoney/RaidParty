// Laget av Hans Ola Hoftun

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"


UCLASS()
class RAIDPARTY_API ABoardPawn : public APawn
{
	GENERATED_BODY()

public:
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentRotationLim_pitch = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentRotationLim_roll = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* PawnMesh{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* PawnMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLinearColor> Colors
		{
			FLinearColor::Red,
			FLinearColor::Blue,
			FLinearColor::Green,
			FLinearColor::Yellow
		};

	UPROPERTY(BlueprintReadWrite)
		bool bIsMoving{ false };

	UPROPERTY(BlueprintReadWrite)
	float Distance = 0;

	UPROPERTY(BlueprintReadWrite)
	float SplineLength = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat* RotationCurve;

	UPROPERTY(EditAnywhere)
	float HeightFactor = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D RotationMinMax {20.f, 110.f};


	UPROPERTY(BlueprintReadWrite)
	class USplineComponent* CurrentSpline {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MovementSpeed = 400;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float SplineMovedPercent = 0;

public:
	UPROPERTY(BlueprintReadOnly)
	FLinearColor PlayerColor;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
		class ABoardSpace* BoardSpace{ nullptr };

	UPROPERTY(BlueprintReadWrite)
	class ACoreLocalPlayerController* PlayerController{ nullptr };
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	virtual FLinearColor SetColor(int index);

	UFUNCTION(BlueprintCallable)
	FLinearColor SetColorLinear(const FLinearColor& color);

	TDelegate<void(ABoardSpace*)> OnPawnArrivedAtNewSpace;

	UFUNCTION(BlueprintImplementableEvent)
	void PawnFinishedMove();

	UFUNCTION(BlueprintImplementableEvent)
	void PawnArrivedAtNewSpace();

// SELECTING PATHS EVENTS
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayPaths(int32 remainingMoves);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePaths(int32 index);

	UFUNCTION(BlueprintImplementableEvent)
	void HidePaths();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMoving() const { return bIsMoving; }

// SELECTING SHRINE EVENTS
	/*
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayShrineOptions();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateShrineOptions(int Selected);

	UFUNCTION(BlueprintImplementableEvent)
	void HideShrineOptions();
	*/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EnterShrineMode();


	UFUNCTION(BlueprintCallable)
	void Move(int index = 0);
};
