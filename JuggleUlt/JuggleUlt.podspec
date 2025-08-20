
Pod::Spec.new do |s|
  s.name             = 'JuggleUlt'
  s.version          = '1.8.19'
  s.summary          = 'Juggle Ultimate.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
  Juggle Ultimate for iOS.
                       DESC

  s.homepage         = 'https://www.juggle.im/'
  s.license          = { :type => "Copyright", :text => "Copyright 2024 Juggle" }
  s.author           = { 'lifei444' => '14392240@qq.com' }
  s.source           = { :http => 'https://downloads.juggle.im/libs/ios/JuggleUlt/JuggleUlt_v1_8_19.zip' }

  s.ios.deployment_target = '12.0'
  
  s.vendored_frameworks = 'JuggleUlt.xcframework'
  
  s.dependency "JuggleIM"
end
