// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "FPSDemoCharacter.h"
#include "FPSDemoProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPSDemo.h"
#include "FPSDemoAttributeComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	SetIsReplicatedByDefault(true);
	bIsFiring = false;
	Mag = MaxMag = 25;
	Bag = 130;
}


void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	if (!GetOwner()->HasAuthority())
	{
		ServerFire();
	}
	bIsFiring = true;
	OnRep_Firing();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTP_WeaponComponent::Stop, 0.2f, false);
}

void UTP_WeaponComponent::ServerFire_Implementation()
{
	Fire();
}

bool UTP_WeaponComponent::AttachWeapon(AFPSDemoCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UTP_WeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// add the weapon as an instance component to the character
	Character->AddInstanceComponent(this);
	Character->SetWeapon(this);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
		}
	}

	return true;
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UTP_WeaponComponent::OnRep_Firing()
{
	if(bIsFiring)
	{
		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
		if (GetOwner()->HasAuthority())
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			//  const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			const FVector SpawnLocation = GetSocketLocation(FName("Muzzle"));
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;

			// Spawn the projectile at the muzzle
			GetWorld()->SpawnActor<AFPSDemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (Mag > 0)
			{
				Mag--;
			}
			else
			{
				int32 next = FMath::Min(MaxMag, Bag);
				Bag -= next;
				Mag = next;
			}
		}
		FString Msg = FString::Printf(TEXT("OnRep_Firing: {%s}"), *GetNameSafe(Character->GetController()));
		LogOnScreen(this, Msg, FColor::Red, 2.0f);
	}
}

void UTP_WeaponComponent::Stop()
{
	bIsFiring = false;
}


void UTP_WeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTP_WeaponComponent, bIsFiring);
}
