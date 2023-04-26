// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardPawn.h"

#include "BoardSpace.h"
#include "CoreLocalPlayerController.h"
#include "Components/SplineComponent.h"

// Sets default values
ABoardPawn::ABoardPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void ABoardPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving || !CurrentSpline)
		return;

	Distance += MovementSpeed * DeltaTime;
	FVector newLocation = CurrentSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	newLocation.Z =  MovementCurve->GetFloatValue(Distance/CurrentSpline->GetSplineLength()) * HeightFactor;
	SetActorLocation(newLocation);

	FRotator newRotation = CurrentSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	newRotation = FRotator(	
		RotationCurve->GetFloatValue(Distance / CurrentSpline->GetSplineLength()) * CurrentRotationLim_pitch,
		CurrentSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Euler().Z,
		RotationCurve->GetFloatValue(Distance / CurrentSpline->GetSplineLength()) * CurrentRotationLim_roll);
	SetActorRotation(newRotation);

	if(Distance >= CurrentSpline->GetSplineLength())
	{
		bIsMoving = false;
		Distance = 0;
		//PawnFinishedMove();
		PawnArrivedAtNewSpace();
		OnPawnArrivedAtNewSpace.ExecuteIfBound(BoardSpace);
	}

}

// Called to bind functionality to input
void ABoardPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FLinearColor ABoardPawn::SetColor(int index)
{
	if (index > Colors.Num() - 1 || !PawnMesh)
		return FLinearColor::Black;

	UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(PawnMaterial, this);
	PlayerColor = Colors[index];
	Mat->SetVectorParameterValue("Color", PlayerColor);
	PawnMesh->SetMaterial(0, Mat);
	return PlayerColor;
}

FLinearColor ABoardPawn::SetColorLinear(const FLinearColor& color)
{
	
	UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(PawnMaterial, this);
	PlayerColor = color;
	Mat->SetVectorParameterValue("Color", PlayerColor);
	PawnMesh->SetMaterial(0, Mat);
	return PlayerColor;

}

void ABoardPawn::Move(int index)
{
	if (bIsMoving || index >= BoardSpace->NextTiles.Num())
		return;

	bIsMoving = true;
	CurrentSpline = BoardSpace->GetPath(index);
	BoardSpace = BoardSpace->NextTiles[index];

	CurrentRotationLim_pitch = FMath::RandRange(-RotationMinMax.X, RotationMinMax.X);
	CurrentRotationLim_roll = FMath::RandRange(-RotationMinMax.Y, RotationMinMax.Y);
}

