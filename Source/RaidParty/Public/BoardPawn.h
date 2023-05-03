// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BoardPawn.generated.h"


UCLASS()
class RAIDPARTY_API ABoardPawn : public APawn
{
	GENERATED_BODY()

	float CurrentRotationLim_pitch = 0.f;
	float CurrentRotationLim_roll = 0.f;

public:
	// Sets default values for this pawn's properties
	ABoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	UPROPERTY(EditAnywhere)
	UCurveFloat* MovementCurve;

	UPROPERTY(EditAnywhere)
	UCurveFloat* RotationCurve;

	UPROPERTY(EditAnywhere)
	float HeightFactor = 200.f;

	UPROPERTY(EditAnywhere)
	FVector2D RotationMinMax {20.f, 110.f};


	UPROPERTY(BlueprintReadWrite)
	class USplineComponent* CurrentSpline {nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MovementSpeed = 400;

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
