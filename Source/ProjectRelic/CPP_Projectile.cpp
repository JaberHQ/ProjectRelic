// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Projectile.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
ACPP_Projectile::ACPP_Projectile()
	:m_sphereRadius( 15.0f )
	,m_initialSpeed( 3000.0f )
	,m_maxSpeed( 3000.0f )
	,m_bounciness( 0.3f )
	,m_projectileGravityScale( 0.0f )
	,m_impulse( 100.0f )
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
		collisionComponent->InitSphereRadius( m_sphereRadius );

		// Set root component to collision component
		RootComponent = collisionComponent;
	}

	if( !projectileMovementComponent )
	{
		// Set projectile movement component
		projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "ProjectileMovementComponent" ) );
		projectileMovementComponent->SetUpdatedComponent( collisionComponent );
		projectileMovementComponent->InitialSpeed = m_initialSpeed;
		projectileMovementComponent->MaxSpeed = m_maxSpeed;
		projectileMovementComponent->bRotationFollowsVelocity = true;
		projectileMovementComponent->bShouldBounce = true;
		projectileMovementComponent->Bounciness = m_bounciness;
		projectileMovementComponent->ProjectileGravityScale = m_projectileGravityScale;
	}

	if( !projectileMeshComponent )
	{
		// Set projectile mesh component
		projectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "ProjectileMeshComponent" ) );

		// Set to sphere
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh( TEXT( "'/Game/Sphere.Sphere'" ) );

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
			//projectileMaterialInstance = UMaterialInstanceDynamic::Create( Material.Object, projectileMeshComponent );
		}

		// Set properties of projectile
		//projectileMeshComponent->SetMaterial( 0, projectileMaterialInstance );
		projectileMeshComponent->SetRelativeScale3D( FVector( 0.09f, 0.09f, 0.09f ) );
		projectileMeshComponent->SetupAttachment( RootComponent );
	}

	//Set Lifespan
	InitialLifeSpan = 3.0f;

	// Set sphere collision
	collisionComponent->BodyInstance.SetCollisionProfileName( TEXT( "Projectile" ) );

	// Collision event
	collisionComponent->OnComponentHit.AddDynamic( this, &ACPP_Projectile::OnHit );

	// Set hit bool to false
	m_isHit = false;
}

// Called when the game starts or when spawned
void ACPP_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Projectile::OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit )
{
	// If other actor is hit and simulating physics
	if( otherActor != this && otherComponent->IsSimulatingPhysics() )
	{
		// Set an impulse and shoot object back
		otherComponent->AddImpulseAtLocation( projectileMovementComponent->Velocity * m_impulse, hit.ImpactPoint );
	}
	// Destroy once hit
	Destroy();
}

void ACPP_Projectile::ShootInDirection( const FVector& shootDirection )
{
	// Set direction and velocity of projectile
	projectileMovementComponent->Velocity = shootDirection * projectileMovementComponent->InitialSpeed;
}

