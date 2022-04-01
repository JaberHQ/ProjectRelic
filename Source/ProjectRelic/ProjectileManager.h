// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ProjectileManager.generated.h"

//UENUM( BlueprintType )
//enum class Type:uint8
//{
//	AssaultRifle UMETA(DisplayName = "AssaultRifle"),
//	Pistol UMETA( DisplayName = "Pistol" ),
//	Fireball UMETA( DisplayName = "Fireball" )
//};

UCLASS()
class PROJECTRELIC_API AProjectileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float deltaTime) override;

	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
	USphereComponent* collisionComponent;

	// Projectile movement component
	UPROPERTY( VisibleAnywhere, Category = Movement )
	UProjectileMovementComponent* projectileMovementComponent;


	// Projectile Mesh
	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
		UStaticMeshComponent* projectileMeshComponent;

	// Projectile Mesh
	UPROPERTY( VisibleDefaultsOnly, Category = Movement )
		UMaterialInstanceDynamic* projectileMaterialInstance;
	
	// When projectile hits something
	UFUNCTION()
		void OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit );

	// Initalise the projectile's velocity 
	void ShootInDirection( const FVector& shootDirection );
};
