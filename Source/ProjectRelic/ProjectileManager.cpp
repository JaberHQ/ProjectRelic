// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManager.h"

// Sets default values
AProjectileManager::AProjectileManager()
{
 	// Set this actor to call Tick() every frame.  
	PrimaryActorTick.bCanEverTick = false;

	if( !RootComponent )
	{
		// Set root component
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
		// Set projectile movement component
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
		// Set projectile mesh component
		projectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "ProjectileMeshComponent" ) );

		// Set to sphere
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh( TEXT( "'/Game/Sphere.Sphere'"));

		// If the reference to mesh succeeded
		if( Mesh.Succeeded() )
		{
			// Set mesh
			projectileMeshComponent->SetStaticMesh( Mesh.Object );
		}
		else
		{
			// Else give error message within log
			UE_LOG( LogTemp, Warning, TEXT( "BAD PATH" ) );
		}

		// Reference to material
		static ConstructorHelpers::FObjectFinder<UMaterial>Material( TEXT( "'/Game/ProjectRelic/Materials/Bullet/SphereMaterial.SphereMaterial'" ) );

		// If reference to material succeeded
		if( Material.Succeeded() )
		{
			// Set material instance
			projectileMaterialInstance = UMaterialInstanceDynamic::Create( Material.Object, projectileMeshComponent );
		}
		// Set material of projectile
		projectileMeshComponent->SetMaterial( 0, projectileMaterialInstance );
		// Set scale of projectile
		projectileMeshComponent->SetRelativeScale3D( FVector( 0.09f, 0.09f, 0.09f ) );
		// Set attachment of projectile
		projectileMeshComponent->SetupAttachment( RootComponent );
	}

	//Set Lifespan
	InitialLifeSpan = 3.0f;

	// Set sphere collision
	collisionComponent->BodyInstance.SetCollisionProfileName( TEXT( "Projectile" ) );

	// Collision event
	collisionComponent->OnComponentHit.AddDynamic( this, &AProjectileManager::OnHit );

	// Set hit bool to false
	isHit = false;
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

	// If other actor is hit and simulating physics
	if( otherActor != this && otherComponent->IsSimulatingPhysics() )
	{
		// Set an impulse and shoot object back
		otherComponent->AddImpulseAtLocation( projectileMovementComponent->Velocity * 100.0f, hit.ImpactPoint );
	}
	// Destroy once hit
	Destroy();
}

void AProjectileManager::ShootInDirection( const FVector& shootDirection )
{
	// Set direction and velocity of projectile
	projectileMovementComponent->Velocity = shootDirection * projectileMovementComponent->InitialSpeed;
}


