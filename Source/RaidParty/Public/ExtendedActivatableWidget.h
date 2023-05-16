// Laget av Hans Ola Hoftun
// Denne klassen blir ikke aktivt brukt i spillet, og blir derfor utelatt når spillet bygge
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ExtendedActivatableWidget.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);

USTRUCT(BlueprintType)
struct FInputActionBindingHandle
{
	GENERATED_BODY()

public:
	FUIActionBindingHandle Handle;
};

UCLASS(meta = (DisableNativeTick))
class RAIDPARTY_API UExtendedActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
		void RegisterBinding(FDataTableRowHandle InputAction, const FInputActionExecutedDelegate& Callback, FInputActionBindingHandle& BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
		void UnregisterBinding(FInputActionBindingHandle BindingHandle);

	UFUNCTION(BlueprintCallable, Category = ExtendedActivatableWidget)
		void UnregisterAllBindings();

private:
	TArray<FUIActionBindingHandle> BindingHandles;
};
