Pod::Spec.new do |s|

  s.name         = "FMDBHelpers"
  s.version      = "0.0.3"
  s.summary      = "SQL helpers for FMDB"
  s.description  = <<-DESC
                   Helper categories for FMDB to execute common SQL statements, and easily
                   detect errors.
                   DESC

  s.homepage          = "https://github.com/nerdyc/FMDBHelpers"
  s.license           = { :type => "MIT", :file => "LICENSE" }
  
  s.author            = { "Christian Niles" => "christian@nerdyc.com" }
  s.social_media_url  = "http://twitter.com/nerdyc"

  s.source            = { :git => "https://github.com/nerdyc/FMDBHelpers.git", :tag => "0.0.3" }
  s.source_files      = "Sources"
  s.requires_arc      = true
  
  s.dependency "FMDB", "~> 2.3"

end
