// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FPSDemoCharacter.generated.h"

class UTP_WeaponComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AFPSDemoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> LandedCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> JumpCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

public:
	AFPSDemoCharacter();
	virtual void PostInitializeComponents() override;

	void SetWeapon(UTP_WeaponComponent* WeaponComponent);

protected:
	UPROPERTY(BlueprintReadOnly)
	UTP_WeaponComponent* WeaponComponent;

	virtual void BeginPlay();

public:
	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnJumped_Implementation() override;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp, int32 NewShield, int32 Delta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UFPSDemoAttributeComponent> AttributeComponent;

	UPROPERTY(ReplicatedUsing="OnRep_IsAlive")
	bool bIsAlive;

	UFUNCTION()
	void OnRep_IsAlive();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
