
Pod::Spec.new do |s|
  s.name             = 'JLiveKitCall'
  s.version          = '1.8.27.2'
  s.summary          = 'Juggle Call With LiveKit.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
  Juggle Call With LiveKit for iOS.
                       DESC

  s.homepage         = 'https://www.juggle.im/'
  s.license          = { :type => "Copyright", :text => "Copyright 2024 Juggle" }
  s.author           = { 'lifei444' => '14392240@qq.com' }
  s.source           = { :http => 'https://downloads.juggle.im/libs/ios/JLiveKitCall/JLiveKitCall_v1_8_27_2.zip' }

  s.ios.deployment_target = '13.0'
  
  s.vendored_frameworks = 'JLiveKitCall.xcframework'
  s.source_files = 'JLiveKitCall/*.{h,m}'
  
  s.dependency "JuggleIM"
  s.dependency "LiveKitClient", "2.8.1"
end
