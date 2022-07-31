// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_CharacterManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <Perception/AISenseConfig_Hearing.h>
#include "DrawDebugHelpers.h"

// Sets default values
ACPP_CharacterManager::ACPP_CharacterManager()
	:m_isCrouched( true )
	,bulletComp()
	,m_projectileRange( 1000.0f )
	,m_muzzleRotationPitch( 3.0f )
	,m_canBeShot( true )
	,m_weaponRange( 20000.0f )
	,m_aimingIn( false )
	,timeBetweenShots( 0.15f )
	,m_shootTime()
	,weaponSocket( TEXT( "GunSocket" ) )
	,muzzleSocket( TEXT( "MuzzleSocket" ) )
	,headSocket( TEXT( "Head" ) )
	,m_reloadTime()
	,m_reloadAnimTime( 3.0f )
	,m_fullMag( 30 )
	,m_assaultRifle ( true )
	,m_pistol( false )
	,m_throwable()
	,recoil( -0.1f )
	,m_shotInHead( false )
	,m_hitmarker( false )
	,m_hitmarkerTimer()
	,m_ammoAR( 30 )
	,m_reserveAR( 0 )
	,m_fullMagAR ( 30 )
	,m_ammoPistol( 15 )
	,m_reservePistol( 0 )
	,m_fullMagPistol( 15 )
	,m_throwableAmount( 1 )
	,animThrow()
	,m_predictionSpline()
	,m_predictionSplineMesh()
	,m_throwSpeed( 2500.0f )
	,m_predictionEndPoint()
	,pointLocation()
	,m_splineIndex(0)
	,cylinderPredictionMesh()
	,m_throwSocket( TEXT( "ThrowableSocket" ) )
	,shootSFX()
	,emptyGunSFX()
	,noiseTag( TEXT( "Noise" ) )
	,m_turnLeft()
	,m_turnRight()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	// Initialise components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	bulletComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BulletComp" ) );
	throwable = CreateDefaultSubobject<UChildActorComponent>( TEXT( "Throwable" ) );
	m_predictionSpline = CreateDefaultSubobject<USplineComponent>( TEXT( "Spline Comp" ) );

	m_throwSceneComp = CreateDefaultSubobject<USceneComponent>( TEXT( "Throw Transform" ) );

	
	// Set relative location and rotation of the skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation( FVector( 0.0f, 0.0f, -90.0f ), FQuat( FRotator( 0.0f, -90.0f, 0.0f ) ) );

	// Attatch components to mesh
	springArmComp->SetupAttachment( GetMesh() );
	cameraComp->SetupAttachment( springArmComp, USpringArmComponent::SocketName );

	// Set class variables of the spring arm
	springArmComp->bUsePawnControlRotation = true;
	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	springArmComp->TargetArmLength = 200.0f;


	//gunComp->AttachTo( GetMesh(), weaponSocket, EAttachLocation::SnapToTargetIncludingScale, false );
	gunComp->SetupAttachment( GetMesh(), weaponSocket );
	bulletComp->SetupAttachment( gunComp );
	//bulletComp->AttachTo( gunComp, weaponSocket, EAttachLocation::SnapToTarget, true );
	//bulletComp->SetRelativeLocation( ( gunComp->GetRelativeLocation() ) );

	// Set class variables of Character Movement Component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	// Set nav agent property for crouching to true
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
}

// Called when the game starts or when spawned
void ACPP_CharacterManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_CharacterManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ACPP_CharacterManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );




}



void ACPP_CharacterManager::UpdateWalkSpeed( float speed )
{
	// Set walk speed
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

FHitResult ACPP_CharacterManager::RaycastShot()
{
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace
	const FVector start = cameraComp->GetComponentLocation();
	const FVector end = ( cameraComp->GetForwardVector() * m_weaponRange ) + start;

	// Draw a line for debug
	//DrawDebugLine( GetWorld(), start, end, FColor::Blue, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( Shoot ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );

	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_WorldDynamic, traceParams );

	// Muzzle offset
	m_muzzleOffset.Set( 200.0f, 0.0f, 0.0f );

	// Transform Muzzleoffset from camera space to world space
	FVector muzzleLocation = cameraLocation + FTransform( cameraRotation ).TransformVector( m_muzzleOffset );

	// Skew aim to be upwards
	FRotator muzzleRotation = cameraRotation;
	muzzleRotation.Pitch += m_muzzleRotationPitch;

	if( bHit )
	{
		// Box where collision has occured
		//DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );
	}

	return hit;
}

int ACPP_CharacterManager::GetSplineIndex()
{
	return m_splineIndex;
}

void ACPP_CharacterManager::ShootProjectile()
{
	if( m_ammoAR > 0  || m_ammoPistol > 0  )
	{
		// Spawn muzzle flash
		UGameplayStatics::SpawnEmitterAttached( animShoot, bulletComp, "MyAttachPoint", bulletComp->GetRelativeLocation(), FRotator( 0.0f, 90.0f, 0.0f ), FVector( 0.1f, 0.1f, 0.1f ) );

		if( m_assaultRifle )
		{
			m_ammoAR -= 1;
			FVector location = GetActorLocation();
			UGameplayStatics::PlaySoundAtLocation( GetWorld(), shootSFX, gunComp->GetRelativeLocation(), 0.3f );
			UAISense_Hearing::ReportNoiseEvent( GetWorld(), location, 1.0f, this, 0.0f, noiseTag );
		}
		if( m_pistol )
		{
			m_ammoPistol -= 1;
		}
	}
	if( m_ammoAR == 0 || m_ammoPistol == 0 )
	{
		UGameplayStatics::PlaySoundAtLocation( GetWorld(), emptyGunSFX, gunComp->GetRelativeLocation(), 1.0f );
	}

	// Get the hit that has been returned
	FHitResult hit = RaycastShot();

	//FString ammoDebug = FString::SanitizeFloat( m_ammoCount );

	// Get the actor that has been hit
	ACPP_CharacterManager* hitActor = Cast<ACPP_CharacterManager>( hit.Actor );

	// If the actor can be shot and has been hit
	if( hitActor && m_canBeShot )
	{

		if( hit.BoneName == headSocket )
		{
			{
				hitActor->HasBeenShotInTheHead( true );
			}
		}

		// Call function that decides what happens when hit 
		hitActor->TakeAttack(); // Function is overridable 

		// Hitmarker
		m_hitmarker = true;
		GetWorld()->GetTimerManager().SetTimer( m_hitmarkerTimer, this, &ACPP_CharacterManager::HitmarkerFinish, 1.0f, true );
	}
}

void ACPP_CharacterManager::TakeAttack()
{
	// -- IMPLEMENTATION REQUIRED --
}


void ACPP_CharacterManager::Reloaded()
{
	GetWorld()->GetTimerManager().ClearTimer( m_reloadTime );
}


void ACPP_CharacterManager::TakeCover()
{
}


void ACPP_CharacterManager::EnemyShoot()
{
	ShootProjectile();
}

bool ACPP_CharacterManager::GetIsAimedIn()
{
	return m_aimingIn;
}

void ACPP_CharacterManager::HasBeenShotInTheHead( bool boolean )
{
	m_shotInHead = boolean;
}

void ACPP_CharacterManager::HitmarkerFinish()
{
	m_hitmarker = false;
}

