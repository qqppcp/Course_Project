// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSDemoHittableActor.generated.h"

class UFPSDemoAttributeComponent;
/**
 * 用于击中交互的AActor，不适用AActor的派生类
 */
UCLASS()
class FPSDEMO_API AFPSDemoHittableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSDemoHittableActor();
	
	virtual void PostInitializeComponents() override;
	
protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, int32 NewShield, int32 Delta);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFPSDemoAttributeComponent> AttributeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameSetting")
	float InitialCubeScale;
	
	UPROPERTY(ReplicatedUsing="OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

};
