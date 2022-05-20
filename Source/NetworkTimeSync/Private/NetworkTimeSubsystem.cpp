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

#include "NetworkTimeSubsystem.h"
#include "NetworkTimeSync.h"

UNetworkTimeSubsystem* UNetworkTimeSubsystem::Get(UObject* WorldContextObject)
{
	ensureMsgf(WorldContextObject, TEXT("Cannot get network time subsystem from a null world context object."));
    if (IsValid(WorldContextObject))
    {
    	if (const UWorld* World = WorldContextObject->GetWorld())
    	{
    		if (const UGameInstance* GI = World->GetGameInstance())
    		{
    			return GI->GetSubsystem<UNetworkTimeSubsystem>();
    		}
    	}
    }
    
	return nullptr;
}

float UNetworkTimeSubsystem::GetServerWorldTime() const
{
	return GetWorld()->GetTimeSeconds() + ServerWorldTimeDelta;
}

void UNetworkTimeSubsystem::OnServerWorldTimeReceived(const float ClientTime, const float ServerTime)
{
	const float RoundTripTime = GetWorld()->GetTimeSeconds() - ClientTime;
	if (RoundTripTime < ShortestRoundTripTime)
	{
		ShortestRoundTripTime = RoundTripTime;
		// Obviously, RTT / 2 isn't representative of all networking conditions since it assumes that RTT is perfectly split 50/50.
		// Better than nothing.
		const float OldDelta = ServerWorldTimeDelta;
		ServerWorldTimeDelta = ServerTime - ClientTime - (ShortestRoundTripTime / 2.0f);
		UE_LOG(LogNetworkTimeSync, Log, TEXT("Received new server world time: %f | ClientTimestamp: %f, ServerTimestamp: %f, RTT: %f, WorldTimeDelta: %f"),
			GetServerWorldTime(), ClientTime, ServerTime, RoundTripTime, ServerWorldTimeDelta);
		
		OnNetworkClockSynchronized.Broadcast(OldDelta, ServerWorldTimeDelta);
	}
}
