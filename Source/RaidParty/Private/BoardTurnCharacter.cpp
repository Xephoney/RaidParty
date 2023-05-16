// Laget av Hans Ola Hoftun

#include "BoardTurnCharacter.h"

// Sets default values
ABoardTurnCharacter::ABoardTurnCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

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

