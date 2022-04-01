// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManager.h"

// Sets default values
AProjectileManager::AProjectileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if( !RootComponent )
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>( TEXT( "ProjectileSceneComponent" ) );
	}

	if( !collisionComponent )
	{
		// Sphere
		collisionComponent = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComponent" ) );
		// Sphere collision radius
		collisionComponent->InitSphereRadius( 15.0f );
		// Set root component to collision component
		RootComponent = collisionComponent;
	}
	
	if( !projectileMovementComponent )
	{
		projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "ProjectileMovementComponent" ) );
		projectileMovementComponent->SetUpdatedComponent( collisionComponent );
		projectileMovementComponent->InitialSpeed = 3000.0f;
		projectileMovementComponent->MaxSpeed = 3000.0f;
		projectileMovementComponent->bRotationFollowsVelocity = true;
		projectileMovementComponent->bShouldBounce = true;
		projectileMovementComponent->Bounciness = 0.3f;
		projectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	
	if( !projectileMeshComponent )
	{
		projectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "ProjectileMeshComponent" ) );
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh( TEXT( "'/Game/Sphere.Sphere'"));
		if( Mesh.Succeeded() )
		{
			projectileMeshComponent->SetStaticMesh( Mesh.Object );
		}
		else
		{
			UE_LOG( LogTemp, Warning, TEXT( "BAD PATH" ) );
		}
		static ConstructorHelpers::FObjectFinder<UMaterial>Material( TEXT( "'/Game/ProjectRelic/Materials/Bullet/SphereMaterial.SphereMaterial'" ) );
		if( Material.Succeeded() )
		{
			projectileMaterialInstance = UMaterialInstanceDynamic::Create( Material.Object, projectileMeshComponent );
		}
		projectileMeshComponent->SetMaterial( 0, projectileMaterialInstance );
		projectileMeshComponent->SetRelativeScale3D( FVector( 0.09f, 0.09f, 0.09f ) );
		projectileMeshComponent->SetupAttachment( RootComponent );
	}
	//Set Lifespan
	InitialLifeSpan = 3.0f;

	// Set sphere collision
	collisionComponent->BodyInstance.SetCollisionProfileName( TEXT( "Projectile" ) );

	// Collision event
	collisionComponent->OnComponentHit.AddDynamic( this, &AProjectileManager::OnHit );
}

// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileManager::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

void AProjectileManager::OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit )
{
	if( otherActor != this && otherComponent->IsSimulatingPhysics() )
	{
		otherComponent->AddImpulseAtLocation( projectileMovementComponent->Velocity * 100.0f, hit.ImpactPoint );
	}

	Destroy();
}

void AProjectileManager::ShootInDirection( const FVector& shootDirection )
{
	projectileMovementComponent->Velocity = shootDirection * projectileMovementComponent->InitialSpeed;
}


