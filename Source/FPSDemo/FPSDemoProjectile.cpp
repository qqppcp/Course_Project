// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSDemoProjectile.h"

#include "FPSDemoAttributeComponent.h"
#include "FPSDemoCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AFPSDemoProjectile::AFPSDemoProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSDemoProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	
	bReplicates = true;
}

void AFPSDemoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO:修改类型的判断，直接判断是否有属性组件
	if (OtherActor == GetInstigator())
	{
		return;
	}
	{
		UFPSDemoAttributeComponent* AttributeComponent = Cast<UFPSDemoAttributeComponent>(OtherActor->GetComponentByClass(UFPSDemoAttributeComponent::StaticClass()));
		if (AttributeComponent != nullptr)
		{
			if (AttributeComponent->IsAlive())
			{
				if (AttributeComponent->GetShield() > 0)
				{
					AttributeComponent->ApplyShieldChange(GetInstigator(), -1);
					UE_LOG(LogTemp, Warning, TEXT("AFPSDemoProjectile::OnHit"));
				}
				else
				{
					AttributeComponent->ApplyHealthChange(GetInstigator(), -1.0f);
				}
			}
			Destroy();
			if ((OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}
			return;
		}
	}
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		UFPSDemoAttributeComponent* AttributeComponent = Cast<UFPSDemoAttributeComponent>(OtherActor->GetComponentByClass(UFPSDemoAttributeComponent::StaticClass()));
		if (AttributeComponent != nullptr)
		{
			if (AttributeComponent->IsAlive())
			{
				if (AttributeComponent->GetShield() > 0)
				{
					AttributeComponent->ApplyShieldChange(GetInstigator(), -1);
					UE_LOG(LogTemp, Warning, TEXT("AFPSDemoProjectile::OnHit"));
				}
				else
				{
					AttributeComponent->ApplyHealthChange(GetInstigator(), -1.0f);
				}
			}
		}
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(5.0f));
		Destroy();
	}
	else if (Cast<AFPSDemoCharacter>(OtherActor) != nullptr)
	{
		UFPSDemoAttributeComponent* AttributeComponent = Cast<UFPSDemoAttributeComponent>(OtherActor->GetComponentByClass(UFPSDemoAttributeComponent::StaticClass()));
		if (AttributeComponent != nullptr)
		{
			if (AttributeComponent->IsAlive())
			{
				if (AttributeComponent->GetShield() > 0)
				{
					AttributeComponent->ApplyShieldChange(GetInstigator(), -1);
					UE_LOG(LogTemp, Warning, TEXT("AFPSDemoProjectile::OnHit"));
				}
				else
				{
					AttributeComponent->ApplyHealthChange(GetInstigator(), -1.0f);
				}
			}
		}
		Destroy();
	}
}