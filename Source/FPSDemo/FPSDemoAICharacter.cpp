// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoAICharacter.h"

#include "FPSDemo.h"
#include "FPSDemoAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSDemoAICharacter::AFPSDemoAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UFPSDemoAttributeComponent>(TEXT("AttributeComp"));
	
	bIsAlive = true;
}

void AFPSDemoAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AFPSDemoAICharacter::OnHealthChanged);
	AttributeComponent->OnShieldChanged.AddDynamic(this, &AFPSDemoAICharacter::OnShieldChanged);
}

// Called when the game starts or when spawned
void AFPSDemoAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSDemoAICharacter::OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
	float NewHealth, float Delta)
{
	if(!OwningComp->IsAlive())
	{
		bIsAlive = false;
		OnRep_IsAlive();
	}
}

void AFPSDemoAICharacter::OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
	int32 NewShield, int32 Delta)
{
	if(!OwningComp->IsAlive())
	{
		bIsAlive = false;
		OnRep_IsAlive();
	}
}

void AFPSDemoAICharacter::DeathStart_Implementation()
{
	float time = PlayAnimMontage(DeathAnimation);
	
	FTimerHandle TimerHandle_AttackDelay;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "Delay_Elapsed");

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, 0.5f, false);
	
}


void AFPSDemoAICharacter::Delay_Elapsed()
{
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
}

void AFPSDemoAICharacter::OnRep_IsAlive()
{
	if(bIsAlive == false)
	{
		DeathStart();
		// float time = PlayAnimMontage(DeathAnimation);
		//
		// GetMesh()->SetAllBodiesSimulatePhysics(true);
		// GetMesh()->SetCollisionProfileName("Ragdoll");
		//
		//
		// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// GetCharacterMovement()->DisableMovement();
	}
}

void AFPSDemoAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSDemoAICharacter, bIsAlive);
}

