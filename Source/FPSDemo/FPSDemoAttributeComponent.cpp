// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSDemoAttributeComponent.h"

#include "FPSDemoGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UFPSDemoAttributeComponent::UFPSDemoAttributeComponent()
{
	Health = 100;
	Shield = 0;

	SetIsReplicatedByDefault(true);
}

void UFPSDemoAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth,
                                                                       float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void UFPSDemoAttributeComponent::MulticastShieldChanged_Implementation(AActor* InstigatorActor, int32 NewShield, int32 Delta)
{
	OnShieldChanged.Broadcast(InstigatorActor, this, NewShield, Delta);
}

bool UFPSDemoAttributeComponent::IsAlive() const
{
	return Health > 0.0f || Shield > 0;
}

float UFPSDemoAttributeComponent::GetHealth() const
{
	return Health;
}

bool UFPSDemoAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	float NewHealth = Health + Delta;
	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		if (!FMath::IsNearlyZero(Delta))
		{
			MulticastHealthChanged(InstigatorActor, NewHealth, Delta);
		}
	}
	return !FMath::IsNearlyZero(Delta);
}

int32 UFPSDemoAttributeComponent::GetShield() const
{
	return Shield;
}

bool UFPSDemoAttributeComponent::ApplyShieldChange(AActor* InstigatorActor, int32 Delta)
{
	int32 NewShield = Shield + Delta;
	if(GetOwner()->HasAuthority())
	{
		Shield = NewShield;
		if (Delta != 0)
		{
			MulticastShieldChanged(InstigatorActor, NewShield, Delta);
		}
		if (!IsAlive())
		{
			AFPSDemoGameMode* GM = GetWorld()->GetAuthGameMode<AFPSDemoGameMode>();
			if (GM)
			{
				GM->OnActorKilled(this->GetOwner(), InstigatorActor);
			}
		}
	}
	return Delta >= 0;
}

bool UFPSDemoAttributeComponent::IsBonus() const
{
	return bIsBonus;
}

void UFPSDemoAttributeComponent::SetBonus()
{
	bIsBonus = true;
}

void UFPSDemoAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFPSDemoAttributeComponent, Health);
	DOREPLIFETIME(UFPSDemoAttributeComponent, Shield);
	DOREPLIFETIME(UFPSDemoAttributeComponent, bIsBonus);
}