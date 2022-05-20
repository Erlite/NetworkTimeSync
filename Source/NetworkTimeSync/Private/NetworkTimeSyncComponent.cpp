/*
MIT License

Copyright (c) 2022 Younes Meziane

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "NetworkTimeSyncComponent.h"

#include "NetworkTimeSubsystem.h"
#include "NetworkTimeSyncSettings.h"

UNetworkTimeSyncComponent::UNetworkTimeSyncComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UNetworkTimeSyncComponent::BeginPlay()
{
	Super::BeginPlay();

	NetworkTimeSubsystem = UNetworkTimeSubsystem::Get(this);
	ensureAlwaysMsgf(NetworkTimeSubsystem.IsValid(), TEXT("Failed to retrieve network time subsystem within UNetworkTimeSyncComponent."));
    
	const UNetworkTimeSyncSettings* Settings = GetDefault<UNetworkTimeSyncSettings>();
	ensureAlwaysMsgf(Settings, TEXT("Failed to retrieve network time sync settings."));

	// This should never happen, but I'd rather avoid crashing your editor.
	if (!NetworkTimeSubsystem.IsValid() || !Settings)
	{
		return;
	}
	
	bUseUnreliableRPCs = Settings->bUseUnreliableRPCs;
	if (IsNetMode(NM_Client))
	{
		GetWorld()->GetTimerManager().SetTimer(Handle_SynchronizeNetworkTime, this, &UNetworkTimeSyncComponent::SynchronizeNetworkTime,
			FMath::Max(Settings->NetworkSyncIntervalSeconds, 1.0f), true);

		SynchronizeNetworkTime();
	}
}

void UNetworkTimeSyncComponent::SynchronizeNetworkTime()
{
	if (bUseUnreliableRPCs)
	{
		Server_UnreliableRequestWorldTime(GetWorld()->GetTimeSeconds());
	}
	else
	{
		Server_ReliableRequestWorldTime(GetWorld()->GetTimeSeconds());
	}
}

void UNetworkTimeSyncComponent::Server_ReliableRequestWorldTime_Implementation(const float ClientTimestamp)
{
	if (bUseUnreliableRPCs)
	{
		Client_UnreliableSendWorldTime(ClientTimestamp, GetWorld()->GetTimeSeconds());
	}
	else
	{
		Client_ReliableSendWorldTime(ClientTimestamp, GetWorld()->GetTimeSeconds());
	}
}

void UNetworkTimeSyncComponent::Server_UnreliableRequestWorldTime_Implementation(const float ClientTimestamp)
{
	if (bUseUnreliableRPCs)
	{
		Client_UnreliableSendWorldTime(ClientTimestamp, GetWorld()->GetTimeSeconds());
	}
	else
	{
		Client_ReliableSendWorldTime(ClientTimestamp, GetWorld()->GetTimeSeconds());
	}
}

void UNetworkTimeSyncComponent::Client_ReliableSendWorldTime_Implementation(const float ClientTimestamp, const float ServerTimestamp)
{
	if (NetworkTimeSubsystem.IsValid())
	{
		NetworkTimeSubsystem->OnServerWorldTimeReceived(ClientTimestamp, ServerTimestamp);
	}
}

void UNetworkTimeSyncComponent::Client_UnreliableSendWorldTime_Implementation(const float ClientTimestamp, const float ServerTimestamp)
{
	if (NetworkTimeSubsystem.IsValid())
	{
		NetworkTimeSubsystem->OnServerWorldTimeReceived(ClientTimestamp, ServerTimestamp);
	}
}