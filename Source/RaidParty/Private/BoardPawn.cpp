// Laget av Hans Ola Hoftun

#include "BoardPawn.h"

#include "BoardSpace.h"
#include "CoreLocalPlayerController.h"
#include "Components/SplineComponent.h"

ABoardPawn::ABoardPawn()
{
 	PrimaryActorTick.bCanEverTick = true;
		
}

void ABoardPawn::BeginPlay()
{
	Super::BeginPlay();

}

void ABoardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving || !CurrentSpline)
		return;

	Distance += MovementSpeed * DeltaTime;
	FVector newLocation = CurrentSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	SplineMovedPercent = Distance / CurrentSpline->GetSplineLength();
	newLocation.Z =  MovementCurve->GetFloatValue(SplineMovedPercent) * HeightFactor;
	SetActorLocation(newLocation);

	FRotator newRotation = CurrentSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	newRotation = FRotator(	
		RotationCurve->GetFloatValue(SplineMovedPercent) * CurrentRotationLim_pitch,
		CurrentSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World).Euler().Z,
		RotationCurve->GetFloatValue(SplineMovedPercent) * CurrentRotationLim_roll);
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

