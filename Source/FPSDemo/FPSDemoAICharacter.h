// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSDemoAICharacter.generated.h"

UCLASS()
class FPSDEMO_API AFPSDemoAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSDemoAICharacter();

	void PostInitializeComponents() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, int32 NewShield, int32 Delta);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFPSDemoAttributeComponent> AttributeComponent;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_IsAlive")
	bool bIsAlive;

	UFUNCTION()
	void Delay_Elapsed();
	
	UFUNCTION(BlueprintNativeEvent)
	void DeathStart();
	
	UFUNCTION()
	void OnRep_IsAlive();

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathAnimation;

};
