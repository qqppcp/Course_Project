// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSDemoCharacter.h"
#include "FPSDemoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPSDemoAttributeComponent.h"
#include "FPSDemoPlayerController.h"
#include "InputActionValue.h"
#include "TP_WeaponComponent.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFPSDemoCharacter

AFPSDemoCharacter::AFPSDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	AttributeComponent = CreateDefaultSubobject<UFPSDemoAttributeComponent>(TEXT("AttributeComp"));
	WeaponComponent = nullptr;
	
	bIsAlive = true;
}

void AFPSDemoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AFPSDemoCharacter::OnHealthChanged);
	AttributeComponent->OnShieldChanged.AddDynamic(this, &AFPSDemoCharacter::OnShieldChanged);
}

void AFPSDemoCharacter::SetWeapon(UTP_WeaponComponent* Component)
{
	WeaponComponent = Component;
}

void AFPSDemoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AFPSDemoCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(LandedCameraShake);

		//UGameplayStatics::PlaySound2D(this, LandedSound);
	}
}

void AFPSDemoCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();
	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(JumpCameraShake);

		//UGameplayStatics::PlaySound2D(this, JumpedSound);
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AFPSDemoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFPSDemoCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFPSDemoCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AFPSDemoCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFPSDemoCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AFPSDemoCharacter::OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
											float NewHealth, float Delta)
{
	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(HitCameraShake);

		//UGameplayStatics::PlaySound2D(this, JumpedSound);
	}
	if(!OwningComp->IsAlive())
	{
		bIsAlive = false;
		OnRep_IsAlive();
	}
}

void AFPSDemoCharacter::OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
	int32 NewShield, int32 Delta)
{
	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		PC->PlayerCameraManager->StartCameraShake(HitCameraShake);

		//UGameplayStatics::PlaySound2D(this, JumpedSound);
	}
	if(!OwningComp->IsAlive())
	{
		bIsAlive = false;
		OnRep_IsAlive();
	}
}

void AFPSDemoCharacter::OnRep_IsAlive()
{
	if (bIsAlive == false)
	{
		this->SetActorEnableCollision(false);
		SetLifeSpan(1.0f);
	}
}

void AFPSDemoCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSDemoCharacter, bIsAlive);
}
