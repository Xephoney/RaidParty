// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardTurnCharacter.h"

// Sets default values
ABoardTurnCharacter::ABoardTurnCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoardTurnCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoardTurnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoardTurnCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoardTurnCharacter::MoveCamera(FVector2D direction)
{
	const FVector WorldDir  = direction.Y * GetActorForwardVector() + direction.X * GetActorRightVector();
	AddMovementInput(WorldDir, 1.f);
}

