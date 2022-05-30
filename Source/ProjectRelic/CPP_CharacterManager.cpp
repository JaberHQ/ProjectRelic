// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CharacterManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACPP_CharacterManager::ACPP_CharacterManager()
	:m_isCrouched( true )
	,m_projectileRange( 1000.0f )
	,m_canBeShot( true )
	,m_muzzleRotationPitch( 3.0f )
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Initialise components
	springArmComp = CreateDefaultSubobject<USpringArmComponent>( TEXT( "SpringArmComp" ) );
	cameraComp = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComp" ) );

	// Set relative location and rotation of the skeletal mesh
	GetMesh()->SetRelativeLocationAndRotation( FVector( 0.0f, 0.0f, -90.0f ), FQuat( FRotator( 0.0f, -90.0f, 0.0f ) ) );

	// Attatch components to mesh
	springArmComp->SetupAttachment( GetMesh() );
	cameraComp->SetupAttachment( springArmComp, USpringArmComponent::SocketName );

	// Set class variables of the spring arm
	springArmComp->bUsePawnControlRotation = true;

	springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );

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
	if( ( Controller != nullptr ) && ( inputAxis != 0.0f ) )
	{
		// Rotation
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation( 0, Rotation.Yaw, 0 );

		// Get forward vector
		const FVector Direction = FRotationMatrix( YawRotation ).GetUnitAxis( EAxis::Y );

		// Add movement in that direction
		AddMovementInput( Direction, inputAxis );
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
	// Set bool to opposite value
	m_isCrouched = !m_isCrouched;

	// If crouched is true, crouch
	if( m_isCrouched == true )
	{
		Crouch();
		springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 140.0f ) );
	}

	// Else, stop crouching
	else
	{
		UnCrouch();
		springArmComp->SetRelativeLocation( FVector( -80.0f, 0.0f, 160.0f ) );
	}
}

void ACPP_CharacterManager::EndCrouch()
{
	UnCrouch();
}


void ACPP_CharacterManager::UpdateWalkSpeed( float speed )
{
	// Set walk speed
	GetCharacterMovement()->MaxWalkSpeed = speed;
}

FHitResult ACPP_CharacterManager::RaycastShot()
{
	FVector cameraLocation;
	FRotator cameraRotation;

	GetController()->GetPlayerViewPoint( cameraLocation, cameraRotation );

	FVector startTrace = cameraLocation;
	FVector endTrace = ( GetActorLocation() + ( GetActorForwardVector() * m_projectileRange ) );
	//endTrace = cameraLocation + ( cameraRotation.Vector() * m_projectileRange );

	FCollisionQueryParams traceParams( SCENE_QUERY_STAT( Shoot ), true, GetInstigator() );

	FHitResult hit( ForceInit );
	//GetWorld()->LineTraceSingleByChannel( hit, startTrace, endTrace, ECC_Visibility, traceParams );
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, startTrace, endTrace, ECC_WorldDynamic, traceParams );
	// Draw a line for debug
	DrawDebugLine( GetWorld(), startTrace, endTrace, FColor::Red, false, 5.0f );
	if( bHit )
	{
		// Box where collision has occured
		DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );
	}
	return hit;

}




//m_muzzleOffset.Set( 100.0f, 0.0f, 0.0f );

	//// Transform Muzzleoffset from camera space to world space
	//FVector muzzleLocation = cameraLocation + FTransform( cameraRotation ).TransformVector( m_muzzleOffset );

	//// Skew aim to be upwards
	//FRotator muzzleRotation = cameraRotation;
	//muzzleRotation.Pitch += m_muzzleRotationPitch;

void ACPP_CharacterManager::ShootProjectile()
{
	
	FHitResult hit = RaycastShot();
	ACPP_CharacterManager* hitActor = Cast<ACPP_CharacterManager>( hit.Actor );
	if( hitActor && m_canBeShot )
	{
		hitActor->TakeAttack();
	}
}

void ACPP_CharacterManager::TakeAttack()
{
	// Take damage
	// Currenthealth -= incomingDamage
	Destroy();
}
