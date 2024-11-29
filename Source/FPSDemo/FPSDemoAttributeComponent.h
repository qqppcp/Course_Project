// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPSDemoAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAttributeChanged, AActor*, InstigatorActor, UFPSDemoAttributeComponent*, OwningComp, float, NewValue, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnIntAttributeChanged, AActor*, InstigatorActor, UFPSDemoAttributeComponent*, OwningComp, int32, NewValue, int32, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSDEMO_API UFPSDemoAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPSDemoAttributeComponent();
	
protected:
	// 血量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attributes")
	float Health;

	// 次数盾
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attributes")
	int32 Shield;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Replicated, Category="Attributes")
	FVector Color;

	UPROPERTY(ReplicatedUsing="OnRep_IsBonus", BlueprintReadOnly, Category="Attributes")
	bool bIsBonus;

	UFUNCTION()
	void OnRep_IsBonus();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHealthChanged(AActor* InstigatorActor, float NewHealth, float Delta);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastShieldChanged(AActor* InstigatorActor, int32 NewShield, int32 Delta);


public:
	
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category="Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	int32 GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	bool ApplyShieldChange(AActor* InstigatorActor, int32 Delta);

	UFUNCTION(BlueprintCallable, Category="Attributes")
	bool IsBonus() const;

	UFUNCTION(BlueprintCallable, Category="Attributes")
	void SetBonus();
	
	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnAttributeChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Attributes")
	FOnIntAttributeChanged OnShieldChanged;
	
};
