// Fill out your copyright notice in the Description page of Project Settings.
#define COLLISION_COVER ECC_GameTraceChannel2

#include "CPP_CharacterManager.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	,m_isInCover()
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
	,m_throwSpeed( 2000.0f )
	,m_predictionEndPoint()
	,pointLocation()
	,m_splineIndex(0)
	,cylinderPredictionMesh()
	,m_throwSocket( TEXT( "ThrowableSocket" ) )
	,shootSFX()
	,emptyGunSFX()
	,noiseTag( TEXT( "Noise" ) )

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialise components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );
	gunComp = CreateDefaultSubobject<USkeletalMeshComponent>( TEXT( "GunComp" ) );
	bulletComp = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "BulletComp" ) );
	headCollision = CreateDefaultSubobject<USphereComponent>( TEXT( "HeadCollision" ) );
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

	headCollision->SetupAttachment( GetMesh(), headSocket );

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


	if( m_isShooting )
	{
		// Recoil
		//AddControllerPitchInput( recoil );
		
		//shootSFX->Play( 0.0f );
	}

	if( m_aimingIn )
	{
		if( throwable )
		{
			//DrawPredictionSpline();
		}
	}
	else
	{
		
	}
}

// Called to bind functionality to input
void ACPP_CharacterManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAxis( "MoveForward", this, &ACPP_CharacterManager::MoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACPP_CharacterManager::MoveRight );
	PlayerInputComponent->BindAxis( "Turn", this, &APawn::AddControllerYawInput );
	PlayerInputComponent->BindAxis( "LookUp", this, &APawn::AddControllerPitchInput );

	PlayerInputComponent->BindAction( "Jump", IE_Pressed, this, &ACharacter::Jump );
	PlayerInputComponent->BindAction( "Jump", IE_Released, this, &ACharacter::StopJumping );
	PlayerInputComponent->BindAction( "Crouch", IE_Pressed, this, &ACPP_CharacterManager::BeginCrouch );
	//PlayerInputComponent->BindAction( "Crouch", IE_Released, this, &ACPP_CharacterManager::EndCrouch );
	PlayerInputComponent->BindAction( "Sprint", IE_Pressed, this, &ACPP_CharacterManager::BeginSprint );
	PlayerInputComponent->BindAction( "Sprint", IE_Released, this, &ACPP_CharacterManager::EndSprint );



}

void ACPP_CharacterManager::MoveForward( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );
		
		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::X );

		// Add movement in that direction
		AddMovementInput( Direction, inputAxis );
	}

}

void ACPP_CharacterManager::MoveRight( float inputAxis )
{
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) && ( m_isInCover == false ) )
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		// Add movement in that direction
		AddMovementInput( Direction, inputAxis );
	}
	else if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) && ( m_isInCover == true ) )
	{
		CoverTrace( inputAxis );
	}
}

void ACPP_CharacterManager::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void ACPP_CharacterManager::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;

}

void ACPP_CharacterManager::BeginCrouch()
{
	//// Set bool to opposite value
	//m_isCrouched = !m_isCrouched;

	//// If crouched is true, crouch
	//if( m_isCrouched == true )
	//{
	//	Crouch();

	//	// Move camera
	//	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 140.0f ) );
	//}

	//// Else, stop crouching
	//else
	//{
	//	UnCrouch();

	//	// Move camera
	//	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	//}
}

void ACPP_CharacterManager::EndCrouch()
{
	//UnCrouch();
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

void ACPP_CharacterManager::StartShooting()
{
	if( m_aimingIn )
	{
		if( m_assaultRifle )
		{
			if( m_ammoAR > 0 )
			{
				m_isShooting = true;

				//m_ammoAR -= 1;

				ShootProjectile();

				GetWorld()->GetTimerManager().SetTimer( m_shootTime, this, &ACPP_CharacterManager::ShootProjectile, timeBetweenShots, true );
			}

			if( m_ammoAR <= 0 )
			{
				m_isShooting = false;

				GetWorld()->GetTimerManager().SetTimer( m_reloadTime, this, &ACPP_CharacterManager::Reloaded, m_reloadAnimTime, true );
			}

		}

		if( m_pistol )
		{
			if( m_ammoPistol > 0 )
			{
				m_isShooting = true;

				//m_ammoPistol -= 1;

				ShootProjectile();
			}
			
		}
		
		if( m_throwable )
		{
			if( m_throwableAmount == 1 )
			{
				ThrowObject();
			}
		}

	}
}

void ACPP_CharacterManager::StopShooting()
{
	m_isShooting = false;
	GetWorld()->GetTimerManager().ClearTimer( m_shootTime );
}

void ACPP_CharacterManager::ThrowObject()
{
	
	ACPP_Throwable* throwableRef = Cast<ACPP_Throwable>( throwable->GetChildActor() );
	if( throwableRef )
	{
		throwableRef->ThrowObject( UKismetMathLibrary::GetForwardVector(GetControlRotation()) );
		m_throwableAmount--;

		if( animThrow )
		{
			PlayAnimMontage( animThrow );
		}
	}
	
}

void ACPP_CharacterManager::CreatePredictionSpline()
{
	//AddSplineComponent

	m_predictionSpline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	RegisterAllComponents();

	//m_predictionEndPoint = GetWorld()->SpawnActor<ACPP_PredictionEndPoint>();

	//m_predictionEndPoint->SetActorHiddenInGame( true );
}

void ACPP_CharacterManager::DestroyPredictionSpline()
{
	if( m_predictionSpline )
	{
		m_predictionSpline->DestroyComponent();

		// Set prediction spline to null?
		m_predictionSpline;

		DestroyPredictionMeshes();

		//m_predictionEndPoint->Destroy();

		//m_predictionEndPoint;
	}
}

void ACPP_CharacterManager::DestroyPredictionMeshes()
{
	for( int i = 0; i < m_predictionSplineMesh.Num(); i++ )
	{
		m_predictionSplineMesh[ i ]->DestroyComponent();
	}
	m_predictionSplineMesh.Empty();
}

void ACPP_CharacterManager::DrawPredictionSpline()
{
	if( m_predictionSpline )
	{
		FVector start = m_throwSceneComp->GetComponentLocation();
		FVector launchVelocity = m_throwSceneComp->GetForwardVector() * m_throwSpeed; // Velocity
		float projectileRadius = 25.0f;
		FHitResult hit( ForceInit );

		FCollisionQueryParams traceParams( SCENE_QUERY_STAT( predict ), true, GetInstigator() );
		
		FPredictProjectilePathParams params;
		params.StartLocation = start;
		params.LaunchVelocity = launchVelocity;
		//params.bTracePath = true;
		params.ProjectileRadius = projectileRadius;
		params.TraceChannel = ECC_WorldDynamic;
		params.bTraceComplex = false;
		//params.ActorsToIgnore = TArray<ACPP_CharacterManager>( GetSelf() )
		params.DrawDebugType = EDrawDebugTrace::ForOneFrame;
		params.DrawDebugTime = 0.0f;
		params.SimFrequency = 15.0f;
		params.MaxSimTime = 2.0f;
		params.OverrideGravityZ = 0.0f;
		FPredictProjectilePathResult pathResult;

		bool bHit = UGameplayStatics::PredictProjectilePath( GetWorld(), params, pathResult );

		if( bHit )
		{
			//m_predictionEndPoint->SetActorHiddenInGame( false );
			//m_predictionEndPoint->SetActorLocation( hit.ImpactPoint, false, &hit, ETeleportType::TeleportPhysics );

			for( auto pathPoint : pathResult.PathData )
			{
				pointLocation.Add( pathPoint.Location );
			}

		}
		else
		{
			//m_predictionEndPoint->SetActorHiddenInGame( true );
		}

		m_predictionSpline->SetSplinePoints( pointLocation, ESplineCoordinateSpace::World );
		DestroyPredictionMeshes();

		//float splineLength = m_predictionSpline->GetSplineLength();
		float splineLength = UKismetMathLibrary::FTrunc( ( m_predictionSpline->GetSplineLength() / 100.0f ) );
		for( int i = 0; i < splineLength; i++ )
		{
			//Add spline mesh component with shape cylinder and forward axis to Z with manual attachment and spline mesh start/end scale to 0.1 
			//m_predictionSplineMesh.Add(splineMeshComponent)
			
			//Set start and end
			FVector startPos = m_predictionSpline->GetLocationAtDistanceAlongSpline( ( i * 100 ), ESplineCoordinateSpace::World );
			FVector startTangent = UKismetMathLibrary::ClampVectorSize( m_predictionSpline->GetTangentAtDistanceAlongSpline( ( i * 100 ), ESplineCoordinateSpace::World ), 0.0f, 100.0f );
			FVector endPos = m_predictionSpline->GetLocationAtDistanceAlongSpline( ( ( i + 1 ) * 100 ), ESplineCoordinateSpace::World );
			FVector endTangent = UKismetMathLibrary::ClampVectorSize( m_predictionSpline->GetTangentAtDistanceAlongSpline( ( ( i + 1 ) * 100 ), ESplineCoordinateSpace::World ), 0.0f, 100.0f );

			m_predictionSplineMesh[ i ]->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			 
			
			m_predictionSplineMesh[ i ]->SetStaticMesh( cylinderPredictionMesh );
			m_predictionSplineMesh[ i ]->SetGenerateOverlapEvents( false );
			m_predictionSplineMesh[ i ]->CanCharacterStepUpOn = ECB_No;
			m_predictionSplineMesh[ i ]->ForwardAxis = ESplineMeshAxis::Z;
			m_predictionSplineMesh[ i ]->AttachToComponent( GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, m_throwSocket );

			m_predictionSplineMesh[ i ]->SetStartAndEnd( startPos, startTangent, endPos, endTangent, true );
			
			m_predictionSplineMesh[ i ]->SetStartScale( FVector2D( 50, 50 ) );
			m_predictionSplineMesh[ i ]->SetEndScale( FVector2D( 50, 50 ) );

			m_predictionSplineMesh.Add( m_predictionSplineMesh[ i ] );
		}
		RegisterAllComponents();
	}
}

int ACPP_CharacterManager::GetSplineIndex()
{
	return m_splineIndex;
}

void ACPP_CharacterManager::StartAim()
{
	m_aimingIn = true;
	springArmComp->TargetArmLength = 100.0f;
	if( m_throwable )
	{
		//CreatePredictionSpline();
	}
}

void ACPP_CharacterManager::StopAim()
{
	m_aimingIn = false;
	springArmComp->TargetArmLength = 200.0f;
	if( m_throwable )
	{
		//DestroyPredictionSpline();
	}
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
			UGameplayStatics::PlaySoundAtLocation( GetWorld(), shootSFX, gunComp->GetRelativeLocation(), 0.3f );
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

bool ACPP_CharacterManager::GetIsCrouched()
{
	return m_isCrouched;
}

void ACPP_CharacterManager::Reloaded()
{
	GetWorld()->GetTimerManager().ClearTimer( m_reloadTime );
}

void ACPP_CharacterManager::StartCover( FHitResult hit )
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( true );
	GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );
	bUseControllerRotationYaw = false;
	m_isInCover = true;

}

void ACPP_CharacterManager::StopCover()
{
	GetCharacterMovement()->SetPlaneConstraintEnabled( false );
	bUseControllerRotationYaw = true;
	m_isInCover = false;
}

void ACPP_CharacterManager::WallTrace()
{
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace
	const FVector start = GetActorLocation();
	const FVector end = ( GetActorForwardVector() * 100.0f ) + GetActorLocation();

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Yellow, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTracer ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

	if( bHit )
	{
		StartCover( hit );
	}
	else
	{
		StopCover();
	}
}

void ACPP_CharacterManager::TakeCover()
{
}

bool ACPP_CharacterManager::CoverTrace( float inputAxis )
{
	bool rightHit = RightCoverTrace();
	bool leftHit = LeftCoverTrace();
	
	if( rightHit && leftHit )
	{
		// Start and end of line trace
		const FVector start = GetActorLocation();
		const FVector end = GetActorLocation() + ( GetCharacterMovement()->GetPlaneConstraintNormal() * 200.0f );

		// Draw a line for debug
		DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

		FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

		// Hit result
		FHitResult hit( ForceInit );
		bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --

		if( bHit )
		{
			GetCharacterMovement()->SetPlaneConstraintNormal( hit.Normal );

			// Rotation
			const FRotator rotation = Controller->GetControlRotation();
			const FRotator yawRotation ( 0.0f, rotation.Yaw, 0.0f );

			// Get forward vector
			const FVector direction = FRotationMatrix( yawRotation ).GetUnitAxis( EAxis::Y );
				
			// Add movement in that direction
			AddMovementInput( direction, inputAxis );

		}
		return bHit;			
	}

	else
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		if( inputAxis == 1.0f && rightHit )
		{
			// Add movement in that direction
			AddMovementInput( Direction, inputAxis );
		}
		else if( inputAxis == -1.0f && leftHit )
		{
			// Add movement in that direction
			AddMovementInput( Direction, inputAxis );
		}

		return false;
	}
	//return false;
}

bool ACPP_CharacterManager::RightCoverTrace()
{
	// Start and end of line trace
	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );
	const FVector start = GetActorLocation() + ( movementDirection * 45.0f );
	const FVector end = ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f ) * 200.0f ) + start;

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool rightHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return rightHit;
}

bool ACPP_CharacterManager::LeftCoverTrace()
{
	// Left 
	FVector cameraLocation = cameraComp->GetComponentLocation();
	FRotator cameraRotation = cameraComp->GetComponentRotation();

	// Start and end of line trace

	FRotator movementVector = UKismetMathLibrary::MakeRotFromX( GetCharacterMovement()->GetPlaneConstraintNormal() );
	FVector movementDirection = UKismetMathLibrary::GetRightVector( movementVector );
	const FVector start = GetActorLocation() + ( movementDirection * 45.0f );
	const FVector end = ( ( GetCharacterMovement()->GetPlaneConstraintNormal() * -1.0f ) * 200.0f ) + start;

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Orange, false, 5.0f );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( WallTrace ), true, GetInstigator() );

	// Hit result
	FHitResult hit( ForceInit );
	bool leftHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECollisionChannel::COLLISION_COVER, traceParams ); // Trace channel cover --
	return leftHit;
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

