[%bs.raw {| require('isomorphic-fetch') |}]; 
/* All available currencies */
[@bs.deriving jsConverter]
type currency = [
  | `NOK
  | `DKK
  | `SEK
  | `USD
  | `AUD
  | `CAD
  | `EUR
  | `CHF
  | `JPY
  | `GBP
];

module ExchangeRateDecode = {
  type exchangeReport = | Opening | Intermediary | PTAXClosing;
  let exchangeReportFromString = value => switch(value) {
    | "Abertura" => Opening
    | "Fechamento PTAX" => PTAXClosing
    | _ => Intermediary
  };
  type exchangeRateEntry = {
    buyParity: float,
    sellParity: float,
    buyPrice: float,
    sellPrice: float,
    date: Js.Date.t,
    report: exchangeReport
  };
  let entry = json => {
    open! Json.Decode;
    {
    buyParity: json |> field("paridadeCompra", float),
    sellParity: json |> field("paridadeVenda", float),
    buyPrice: json |> field("cotacaoCompra", float),
    sellPrice: json |> field("cotacaoVenda", float),
    date: json |> field("dataHoraCotacao", string) |> Js.Date.fromString,
    report: json |> field("tipoBoletim", string) |> exchangeReportFromString
    }
  };
  let decode = json => {
    Json.Decode.(json |> field("value", array(entry)))
  }
};


let getFetchURL = (~currency: currency, ~date) => {
  let currencyStr = currencyToJs(currency);
  let formattedDate = MomentRe.(date |> momentWithDate |> Moment.format("MM-DD-YYYY"));
  {j|https://olinda.bcb.gov.br/olinda/servico/PTAX/versao/v1/odata/CotacaoMoedaDia(moeda=@moeda,dataCotacao=@dataCotacao)?%40moeda=%27$currencyStr%27&%40dataCotacao=%27$formattedDate%27&%24format=json|j}
};


let fetchCurrencyRatesForPeriod = (~currency: currency, ~date) => {
  open Js.Promise;
  open BsAbstract;

  getFetchURL(~currency, ~date)
  |> Fetch.fetch
  |> then_(Fetch.Response.text)
  |> then_(Function.Infix.( Json.parseOrRaise >. ExchangeRateDecode.decode >. resolve ))
};
