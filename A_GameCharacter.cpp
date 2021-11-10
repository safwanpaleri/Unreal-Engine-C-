


#include "A_GameCharacter.h"
#include "AGun.h"
#include "Components/CapsuleComponent.h"
#include "TPShooterGameModeBase.h"

// Sets default values
AA_GameCharacter::AA_GameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_GameCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AAGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"),EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	
}

// Called every frame
void AA_GameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA_GameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AA_GameCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideward"),this,&AA_GameCharacter::MoveSideward);

	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&AA_GameCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookSides"),this,&AA_GameCharacter::LookSides);

	PlayerInputComponent->BindAction(TEXT("jump"),EInputEvent::IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"),EInputEvent::IE_Pressed,this,&AA_GameCharacter::shoot);
}

void AA_GameCharacter:: MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AA_GameCharacter:: MoveSideward(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AA_GameCharacter:: LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AA_GameCharacter:: LookSides(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AA_GameCharacter::shoot()
{
	Gun->TriggerGun();
}

bool AA_GameCharacter :: IsDead() const
{
	return Health<=0;
}

float AA_GameCharacter :: TakeDamage(float DamageValue,struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageValue, DamageEvent, EventInstigator, DamageCauser);
	Health-=DamageApplied;
	if(IsDead())
	{
		ATPShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATPShooterGameModeBase>();
		if(GameMode != nullptr){GameMode->PawnKilled(this);}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}

