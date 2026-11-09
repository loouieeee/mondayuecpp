#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class MONDAYUECPPPROJECT_API IInteractableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
    void Interact();
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
    void ShowInteractWidget(bool _bShow);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
    void ShowHighlight(bool _bShow);
};