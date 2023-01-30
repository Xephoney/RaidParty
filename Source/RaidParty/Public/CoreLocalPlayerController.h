// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "D:/UE5/UE_5.1/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"

#include "CoreLocalPlayerController.generated.h"

/**
 * 
 */
class UInputAction;
class UInputMappingContext;

UCLASS()
class RAIDPARTY_API ACoreLocalPlayerController : public APlayerController
{
	GENERATED_BODY()
	ACoreLocalPlayerController();
public:
	virtual void BeginPlay() override;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Controls|Input Actions")
	//UInputAction* MovementAction {nullptr};

	//UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Controls|Input Actions")
	//UInputAction* ConfirmAction {nullptr};

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controls|Input Actions")
	//UInputMappingContext* InputMapping {nullptr};

	//void EnhancedMove(const FInputActionValue& value);

protected:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

};
