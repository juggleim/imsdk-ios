#
# Be sure to run `pod lib lint JugglePre.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'JuggleIM'
  s.version          = '1.8.15.3'
  s.summary          = 'Juggle IM SDK.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
Juggle IM SDK for iOS.
                       DESC

  s.homepage         = 'https://www.juggle.im/'
  s.license          = { :type => "Copyright", :text => "Copyright 2024 Juggle" }
  s.author           = { 'lifei444' => '14392240@qq.com' }
  s.source           = { :http => 'https://downloads.juggle.im/libs/ios/JuggleIM_v1_8_15_3.zip' }

  s.ios.deployment_target = '12.0'
  
  s.vendored_frameworks = 'JuggleIM.xcframework'
end
