// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoHittableActor.h"

#include "FPSDemoAttributeComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSDemoHittableActor::AFPSDemoHittableActor()
{

	AttributeComponent = CreateDefaultSubobject<UFPSDemoAttributeComponent>(TEXT("AttributeComp"));
	
 	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	
	InitialCubeScale = 1.1f;
	bIsActive = true;
	bReplicates = true;
}

void AFPSDemoHittableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AFPSDemoHittableActor::OnHealthChanged);
	AttributeComponent->OnShieldChanged.AddDynamic(this, &AFPSDemoHittableActor::OnShieldChanged);
}

void AFPSDemoHittableActor::OnHealthChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
                                            float NewHealth, float Delta)
{
	if(!OwningComp->IsActive())
	{
		bIsActive = false;
		OnRep_IsActive();
	}
}

void AFPSDemoHittableActor::OnShieldChanged(AActor* InstigatorActor, UFPSDemoAttributeComponent* OwningComp,
	int32 NewShield, int32 Delta)
{
	if(Delta < 0)
	{
		FVector Scale = this->GetActorScale();
		this->SetActorScale3D(Scale * InitialCubeScale);
		
	}
	if(!OwningComp->IsAlive())
	{
		
		bIsActive = false;
		OnRep_IsActive();
	}
}

void AFPSDemoHittableActor::OnRep_IsActive()
{
	if (false == bIsActive)
	{
		this->SetActorEnableCollision(false);
		MeshComp->SetCollisionProfileName("Ragdoll");
		SetLifeSpan(1.0f);
	}
}

void AFPSDemoHittableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFPSDemoHittableActor, bIsActive);
}
