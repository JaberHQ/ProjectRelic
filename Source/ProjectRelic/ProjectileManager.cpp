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

	if( !CollisionComponent )
	{
		// Sphere
		CollisionComponent = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComponent" ) );
		// Sphere collision radius
		CollisionComponent->InitSphereRadius( 15.0f );
		// Set root component to collision component
		RootComponent = CollisionComponent;
	}
	
	if( !ProjectileMovementComponent )
	{
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "ProjectileMovementComponent" ) );
		ProjectileMovementComponent->SetUpdatedComponent( CollisionComponent );
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	
	if( !ProjectileMeshComponent )
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "ProjectileMeshComponent" ) );
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh( TEXT( "'/Game/Sphere.Sphere'"));
		if( Mesh.Succeeded() )
		{
			ProjectileMeshComponent->SetStaticMesh( Mesh.Object );
		}
		static ConstructorHelpers::FObjectFinder<UMaterial>Material( TEXT( "'/Game/ProjectRelic/Materials/Bullet/SphereMaterial.SphereMaterial'" ) );
		if( Material.Succeeded() )
		{
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create( Material.Object, ProjectileMeshComponent );
		}
		ProjectileMeshComponent->SetMaterial( 0, ProjectileMaterialInstance );
		ProjectileMeshComponent->SetRelativeScale3D( FVector( 0.09f, 0.09f, 0.09f ) );
		ProjectileMeshComponent->SetupAttachment( RootComponent );
	}

}

// Called when the game starts or when spawned
void AProjectileManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileManager::ShootInDirection( const FVector& ShootDirection )
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


